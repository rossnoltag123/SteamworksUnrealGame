// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameOverlay.h"
#include "Components/Button.h"

bool UInGameOverlay::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	//if (!ensure(JoinButton != nullptr))return;
	OverlayButton->OnClicked.AddDynamic(this, &UInGameOverlay::OverlayBackToMainMenu);

	//if (!ensure(JoinButton != nullptr))return;
	OverlayCancelButton->OnClicked.AddDynamic(this, &UInGameOverlay::OverlayCancel);

	return true;
}

//Open in game menu during gameplay
void UInGameOverlay::OverlayBackToMainMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("OverlayBacktoMAin.........."));
	if (MenuInterface != nullptr)
	{
		RemoveMainMenu();
		MenuInterface->LoadMainMenu();
	}
}

//In game overlay menu cancel button function
void UInGameOverlay::OverlayCancel()
{
	RemoveMainMenu();
}