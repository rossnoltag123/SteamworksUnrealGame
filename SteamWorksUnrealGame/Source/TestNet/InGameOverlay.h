// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.h"
#include "InGameOverlay.generated.h"

/**
 * 
 */
UCLASS()
class TESTNET_API UInGameOverlay : public UMenuWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize();

private:
	//In game overlay menu buttons
	UPROPERTY(meta = (BindWidget))
	class UButton* OverlayCancelButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* OverlayButton;

	UFUNCTION()
		void OverlayBackToMainMenu();

	UFUNCTION()
		void OverlayCancel();
};
