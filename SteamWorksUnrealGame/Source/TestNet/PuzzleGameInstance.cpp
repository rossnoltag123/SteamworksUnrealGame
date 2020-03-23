// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleGameInstance.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Classes/GameFramework/PlayerController.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"

#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.h"
#include "InGameOverlay.h"
#include "MenuWidget.h"

#include <IPAddress.h>
#include <SocketSubsystem.h>

//Global Constant.. not good
const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTING_KEY = TEXT("TestKey");

//Dont need to use virtual infront of constructors...?
UPuzzleGameInstance::UPuzzleGameInstance(const FObjectInitializer & ObjectInitializer){

	//FClass finder is used for finding a blueprint class, same as #include for c++ or GetComponent(Unity) for c#
	ConstructorHelpers::FClassFinder<UUserWidget>BP_MainMenu(TEXT("/Game/PuzzlePlatform/MenuSystem/WBP_MainMenu"));
	if (!ensure(BP_MainMenu.Class != nullptr))return;	//UE_LOG(LogTemp, Warning, TEXT("CLASS PROPERTY..."));
	MainMenu_ClassProperty = BP_MainMenu.Class; //Pointer               //Class Property of where the pointer is pointing to
	UE_LOG(LogTemp, Warning, TEXT("Found class................. %s"), *MainMenu_ClassProperty->GetName())

	ConstructorHelpers::FClassFinder<UUserWidget>BP_InGameOverlay(TEXT("/Game/PuzzlePlatform/MenuSystem/WBP_InGameMenu"));
	if (!ensure(BP_InGameOverlay.Class != nullptr))return;	//UE_LOG(LogTemp, Warning, TEXT("CLASS PROPERTY..."));
	InGameOverlay_ClassProperty = BP_InGameOverlay.Class; //Pointer               //Class Property of where the pointer is pointing to
	UE_LOG(LogTemp, Warning, TEXT("Found class................. %s"), *InGameOverlay_ClassProperty->GetName())

	//ConstructorHelpers::FClassFinder<UUserWidget>BP_Servers(TEXT("/Game/PuzzlePlatform/MenuSystem/WBP_Servers"));
	//if (!ensure(BP_Servers.Class != nullptr))return;	//UE_LOG(LogTemp, Warning, TEXT("CLASS PROPERTY..."));
	//Servers_ClassProperty = BP_Servers.Class; //Pointer               //Class Property of where the pointer is pointing to
	//UE_LOG(LogTemp, Warning, TEXT("Found class................. %s"), *Servers_ClassProperty->GetName())
}

void UPuzzleGameInstance::Init()
{
	IOnlineSubsystem *Subsystem = IOnlineSubsystem::Get();
		if (Subsystem != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Online Subsystem found %s"),*Subsystem->GetSubsystemName().ToString());
			SessionInterface = Subsystem->GetSessionInterface();
			if (SessionInterface.IsValid())
			{
				SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnCreateSessionComplete);
				SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnDestroySessionComplete);
				SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnFindSessionsComplete);
				SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnJoinSessionsComplete);
			}
		}else{ 
			UE_LOG(LogTemp, Warning, TEXT("Online Subsystem not found"));
		}
}

void UPuzzleGameInstance::LoadMenuWidget() {
	if (!ensure(MainMenu_ClassProperty != nullptr)) { return; }
	Menu = CreateWidget<UMainMenu>(this, MainMenu_ClassProperty);
	if (!ensure(Menu != nullptr)) { return; }
	Menu->SetupMainMenu();
	Menu->SetMenuInterface(this);
}

void UPuzzleGameInstance::LoadMainMenu() {
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr))return;
	PlayerController->ClientTravel("/Game/PuzzlePlatform/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPuzzleGameInstance::InGameLoadMenu() {
	if (!ensure(InGameOverlay_ClassProperty != nullptr)) { return; }
	UMenuWidget* Menu = CreateWidget<UMenuWidget>(this, InGameOverlay_ClassProperty);
	if (!ensure(Menu != nullptr)) { return; }
	Menu->SetupMainMenu();
	Menu->SetMenuInterface(this);
}

void UPuzzleGameInstance::StartSession() {
	if (SessionInterface.IsValid()) {
		SessionInterface->StartSession(SESSION_NAME);
	}
}


void UPuzzleGameInstance::SearchSession() {//TSharedPtr<class FOnlineSessionSearch> FindSession
	FindSession = MakeShareable(new FOnlineSessionSearch());
	if (FindSession.IsValid()) {
		//FindSession->bIsLanQuery = true;//Looking for internet session not lan.
		FindSession->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);//This is found in the engine code.Had to dig through it
		FindSession->MaxSearchResults = 100;
		UE_LOG(LogTemp, Warning, TEXT("Finding Session..."));
		SessionInterface->FindSessions(0, FindSession.ToSharedRef());
	}
}

void UPuzzleGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else {
			SessionSettings.bIsLANMatch = false;
		}
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.NumPublicConnections = 5;
		SessionSettings.bUsesPresence = true;//Lobby system for steam
		SessionSettings.Set(SERVER_NAME_SETTING_KEY, IdealServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		UE_LOG(LogTemp, Warning, TEXT("Create %s"), *IdealServerName);
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UPuzzleGameInstance::DestroySession() {

	UE_LOG(LogTemp, Warning, TEXT("Destroying Session"));
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);

		if (ExistingSession != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Destroying Session call back called"));
			SessionInterface->DestroySession(SESSION_NAME);
		}else{
			UE_LOG(LogTemp, Warning, TEXT("ExistingSession = to null pointer"));
			CreateSession();
		}
	}
}

void UPuzzleGameInstance::JoinSession(uint32 Index) {
	if (!SessionInterface.IsValid())return;
	if (!FindSession.IsValid())return;
	if (Menu != nullptr)Menu->RemoveMainMenu();
	SessionInterface->JoinSession(0, SESSION_NAME, FindSession->SearchResults[Index]);
}

void UPuzzleGameInstance::RefreshServerList() {
	//Find Sessions
	//FindSession = MakeShareable(new FOnlineSessionSearch());
	UE_LOG(LogTemp, Warning, TEXT("Searching for sessions"));
	SearchSession();// FindSession
}

void UPuzzleGameInstance::OnFindSessionsComplete(bool bWasSuccessful) {

	if (bWasSuccessful&& FindSession.IsValid()) {
		UE_LOG(LogTemp, Warning, TEXT("SessionSearch Is Valid %d"), FindSession->SearchResults.Num());

		TArray<FServerData> ServerNames;
		for (const FOnlineSessionSearchResult& SearchResult : FindSession->SearchResults)
		{
			FServerData Data;
			Data.Name = SearchResult.GetSessionIdStr();
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;

			//Num open public connections can be resolved through the tutorial 73. Bougs in Steam OSS
			//The number of current players wont be displayed in the join menu.
			Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			Data.HostUsername = SearchResult.Session.OwningUserName;

			FString ServerName;
			
			if(SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTING_KEY, ServerName))
			{
				Data.Name = ServerName;
			}else{
				Data.Name = "Could not find name";
			}
			ServerNames.Add(Data);
		}
		Menu->SetServerList(ServerNames);
	}
}

void UPuzzleGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) {

	if (!bWasSuccessful) {
		UE_LOG(LogTemp, Warning, TEXT("Create session returned false"));
		return;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, FString::Printf(TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));
		ServerTravel();
	}
}

void UPuzzleGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, FString::Printf(TEXT("OnDestroySessionDestroyed %s, %d"), *SessionName.ToString(), bWasSuccessful));
		UE_LOG(LogTemp, Warning, TEXT("Destroy session returned true"));
		CreateSession();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroy session returned false"));
		return;
	}
}

void UPuzzleGameInstance::OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid())return;
	FString Address;

	if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) {
		//Somthingh going on here about Address,out param? returns somthing, chieck it out.
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string"));
		return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr))return;
	Engine->AddOnScreenDebugMessage(0, 5, FColor::Blue, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr))return;
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);//192....
}

void UPuzzleGameInstance::Host(FString ServerName)
{
	IdealServerName = ServerName;
	UE_LOG(LogTemp, Warning, TEXT("Host %s"), *IdealServerName);
	if(SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr) 
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}else{
			CreateSession();
		}
	}
}

void UPuzzleGameInstance::ServerTravel(){
	if (Menu != nullptr) {
		Menu->RemoveMainMenu();}

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr))return;
	const FString& ServerAddress = GetHost(World);
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr))return;
	Engine->AddOnScreenDebugMessage(0, 10, FColor::Blue, TEXT("Hosting") + ServerAddress);
	World->ServerTravel("/Game/PuzzlePlatform/Maps/Lobby?listen");
}

const FString UPuzzleGameInstance::GetHost(UObject* WorldContextObject){
	if (WorldContextObject){
		if (UWorld* World = WorldContextObject->GetWorld()){
			bool canBind = false;
			TSharedRef<FInternetAddr> localIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);
			return (localIp->IsValid() ? localIp->ToString(false) : World->URL.Host);}}
	return "WorldContextObject is FALSE";
}

















