// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Servers.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/TextBlock.h"
#include "UObject/ConstructorHelpers.h"

#include <IPAddress.h>
#include <SocketSubsystem.h>

// Object initializer
UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer) 
{
	ConstructorHelpers::FClassFinder<UUserWidget>BP_Servers(TEXT("/Game/PuzzlePlatform/MenuSystem/WBP_Servers"));
	if (!ensure(BP_Servers.Class != nullptr))return;
	BP_Servers_ClassProperty = BP_Servers.Class;
	UE_LOG(LogTemp, Warning, TEXT("Found class................. %s"), *BP_Servers_ClassProperty->GetName())
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false; 
	//if (!ensure(HostButton != nullptr))return;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	//if (!ensure(JoinMenuButton!= nullptr))return;
	JoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	//if (!ensure(BackButton != nullptr))return;
	BackButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	//if (!ensure(JoinButton != nullptr))return;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	//if (!ensure(JoinButton != nullptr))return;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
	//if (!ensure(JoinButton != nullptr))return;
	RefreshButton->OnClicked.AddDynamic(this, &UMainMenu::RefreshServerList);

	return true;
}

//Host server from main menu
void UMainMenu::HostServer(){
	if (MenuInterface != nullptr){
		MenuInterface->Host();
	}}

//Open join menu from main menu
void UMainMenu::OpenJoinMenu(){
	if (!ensure(WidgetSwitcher != nullptr))return;
	if (!ensure(JoinMenu!= nullptr))return;
	WidgetSwitcher->SetActiveWidget(JoinMenu);

	//if (MenuInterface != nullptr) {
		RefreshServerList();
//	}
}

//Open main menu from join menu
void UMainMenu::OpenMainMenu(){
	if (!ensure(WidgetSwitcher != nullptr))return;
	if (!ensure(MainMenu != nullptr))return;
	WidgetSwitcher->SetActiveWidget(MainMenu);}

void UMainMenu::SetServerList(TArray<FString>ServerNames)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)){return;}

	ScrollBox_Servers->ClearChildren();

	//UServers* ServerRow = CreateWidget<UServers>(World, BP_Servers_ClassProperty);
	//if (!ensure(ServerRow != nullptr)){return;}

	uint32 i = 0;

	for (const FString& ServerName :ServerNames)
	{
		UServers* ServerRow = CreateWidget<UServers>(World, BP_Servers_ClassProperty);
		if (!ensure(ServerRow != nullptr)){return;}
		ServerRow->ServerName->SetText(FText::FromString(ServerName));
		ServerRow->Setup(this, i);
		++i;
		ScrollBox_Servers->AddChild(ServerRow);
	}

}

void UMainMenu::RefreshServerList(){	
	if (MenuInterface != nullptr) {
		MenuInterface->RefreshServerList();
	}
}





void  UMainMenu::SelectIndex(uint32 Index) {
	SelectedIndex = Index;
}

void UMainMenu::JoinServer(){
	if (SelectedIndex.IsSet() && MenuInterface!= nullptr){
		UE_LOG(LogTemp, Warning, TEXT("Selected index %d"), SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else{
		UE_LOG(LogTemp, Warning, TEXT("Selected index not set"));
	}
}






void UMainMenu::QuitGame() {
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr))return;
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr))return;
	PlayerController->ConsoleCommand("quit");
}

const FString UMainMenu::GetHostAddress(UObject* WorldContextObject) {
	if (WorldContextObject) {
		if (UWorld* World = WorldContextObject->GetWorld()) {
			bool canBind = false;
			TSharedRef<FInternetAddr> localIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);
			return (localIp->IsValid() ? localIp->ToString(false) : World->URL.Host);
		}
	}
	return "WorldContextObject is FALSE";
}

