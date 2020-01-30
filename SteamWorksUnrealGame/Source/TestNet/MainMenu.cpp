// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

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

	return true;
}

//Host server from main menu
void UMainMenu::HostServer(){
	if (MenuInterface != nullptr){
		MenuInterface->Host();}}

//Open join menu from main menu
void UMainMenu::OpenJoinMenu(){
	if (!ensure(WidgetSwitcher != nullptr))return;
	if (!ensure(JoinMenu!= nullptr))return;
	WidgetSwitcher->SetActiveWidget(JoinMenu);}

//Open main menu from join menu
void UMainMenu::OpenMainMenu(){
	if (!ensure(WidgetSwitcher != nullptr))return;
	if (!ensure(MainMenu != nullptr))return;
	WidgetSwitcher->SetActiveWidget(MainMenu);}

//Join server from join menu
void UMainMenu::JoinServer(){
	if (MenuInterface != nullptr){
		//Const FString& for memory saving
		const FString& Address = IPAddressField->GetText().ToString();
		MenuInterface->Join(Address);}}

void UMainMenu::QuitGame() {

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr))return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr))return;

	PlayerController->ConsoleCommand("quit");
}
