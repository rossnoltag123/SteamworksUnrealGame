// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuWidget.h"

void UMenuWidget::SetupMainMenu()
{
	this->AddToViewport();
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr))return;
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr))return;
	this->bIsFocusable = true;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

//Remove menu when starting level
void UMenuWidget::RemoveMainMenu()
{
	this->RemoveFromViewport();
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr))return;
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr))return;
	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}

//Set menu interface 
void UMenuWidget::SetMenuInterface(IMenuInterface* MenuInterface)
{
	this->MenuInterface = MenuInterface;
}