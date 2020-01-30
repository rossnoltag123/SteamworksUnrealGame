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
const static FName SESSION_NAME = TEXT("My Session Game");

//Dont need to use virtual infront of constructors...?
UPuzzleGameInstance::UPuzzleGameInstance(const FObjectInitializer & ObjectInitializer){
	ConstructorHelpers::FClassFinder<UUserWidget>BP_MainMenu(TEXT("/Game/PuzzlePlatform/MenuSystem/WBP_MainMenu"));
	if (!ensure(BP_MainMenu.Class != nullptr))return;	//UE_LOG(LogTemp, Warning, TEXT("CLASS PROPERTY..."));
	MainMenu_ClassProperty = BP_MainMenu.Class; //Pointer               //Class Property of where the pointer is pointing to
	UE_LOG(LogTemp, Warning, TEXT("Found class................. %s"), *MainMenu_ClassProperty->GetName())

	ConstructorHelpers::FClassFinder<UUserWidget>BP_InGameOverlay(TEXT("/Game/PuzzlePlatform/MenuSystem/WBP_InGameMenu"));
	if (!ensure(BP_InGameOverlay.Class != nullptr))return;	//UE_LOG(LogTemp, Warning, TEXT("CLASS PROPERTY..."));
	InGameOverlay_ClassProperty = BP_InGameOverlay.Class; //Pointer               //Class Property of where the pointer is pointing to
	UE_LOG(LogTemp, Warning, TEXT("Found class................. %s"), *InGameOverlay_ClassProperty->GetName())
}

void UPuzzleGameInstance::Init() {
	IOnlineSubsystem *Subsystem = IOnlineSubsystem::Get();
		if (Subsystem != nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Online Subsystem found %s"),*Subsystem->GetSubsystemName().ToString());
			SessionInterface = Subsystem->GetSessionInterface();
			if (SessionInterface.IsValid()) {
				UE_LOG(LogTemp, Warning, TEXT("INIT SET UP...1"));

				SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnCreateSessionComplete);
				SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnDestroySessionComplete);

				SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnFindSessionsComplete);

				//check later or morning alot going on here!!!
				SessionSearch = MakeShareable(new FOnlineSessionSearch());

				if (SessionSearch.IsValid()) {
					UE_LOG(LogTemp, Warning, TEXT("Finding Session..."));
					SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
				}
			}
		}
		else
			{ UE_LOG(LogTemp, Warning, TEXT("Online Subsystem not found"));
		}
}


void UPuzzleGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{

	UE_LOG(LogTemp, Warning, TEXT("Find Session Complete:%d"), bWasSuccessful);

}

void UPuzzleGameInstance::LoadMenuWidget(){
	if (!ensure(MainMenu_ClassProperty != nullptr)) {return;}
	Menu = CreateWidget<UMainMenu>(this, MainMenu_ClassProperty);
	if (!ensure(Menu != nullptr)) {return;}
	Menu->SetupMainMenu();
	Menu->SetMenuInterface(this);
}

void UPuzzleGameInstance::Host()
{

	if(SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Session Interrface is valid"));
		DestroySession();
	}
	else{
		UE_LOG(LogTemp, Warning, TEXT("Session Interrface is not valid"));
		CreateSession();
	}
}

void UPuzzleGameInstance::CreateSession(){
	UE_LOG(LogTemp, Warning, TEXT("Creating session..."));
	FOnlineSessionSettings SessionSettings;
	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

void UPuzzleGameInstance::DestroySession() {
	UE_LOG(LogTemp, Warning, TEXT("Destroying Session"));

	if (SessionInterface.IsValid()) {

		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);

		if(ExistingSession != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Destroying Session call back called"));
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else 
		{ 
			UE_LOG(LogTemp, Warning, TEXT("ExistingSession = to null pointer")); 
			CreateSession();
		}
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

void UPuzzleGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful) {

	if (bWasSuccessful) {
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, FString::Printf(TEXT("OnDestroySessionDestroyed %s, %d"), *SessionName.ToString(), bWasSuccessful));
		UE_LOG(LogTemp, Warning, TEXT("Destroy session returned true"));
		CreateSession();
	}
	else{
		UE_LOG(LogTemp, Warning, TEXT("Destroy session returned false"));
		return;
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
	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzleGameInstance::Join(const FString& Address){
	if (Menu != nullptr){
		Menu->SetupMainMenu();}
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr))return;
	Engine->AddOnScreenDebugMessage(0, 5, FColor::Blue, FString::Printf(TEXT("Joining %s"), *Address));
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr))return;
	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);//192....
}

void UPuzzleGameInstance::InGameLoadMenu(){
	if (!ensure(InGameOverlay_ClassProperty != nullptr)) { return; }
	UMenuWidget* Menu = CreateWidget<UMenuWidget>(this, InGameOverlay_ClassProperty);
	if (!ensure(Menu != nullptr)) { return; }
	Menu->SetupMainMenu();
	Menu->SetMenuInterface(this);
}

void UPuzzleGameInstance::LoadMainMenu(){
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr))return;
	PlayerController->ClientTravel("/Game/PuzzlePlatform/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

const FString UPuzzleGameInstance::GetHost(UObject* WorldContextObject){
	if (WorldContextObject){
		if (UWorld* World = WorldContextObject->GetWorld()){
			bool canBind = false;
			TSharedRef<FInternetAddr> localIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);
			return (localIp->IsValid() ? localIp->ToString(false) : World->URL.Host);}}
	return "WorldContextObject is FALSE";
}

/*
APlayerController* PlayerController = World->GetFirstPlayerController();
if (!ensure(PlayerController != nullptr))return;
FInputModeGameOnly InputModeData;
PlayerController->SetInputMode(InputModeData);
PlayerController->bShowMouseCursor = false;
*/