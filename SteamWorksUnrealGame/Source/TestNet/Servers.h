// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Servers.generated.h"

/**
 * 
 */
UCLASS()
class TESTNET_API UServers : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;

	void Setup(class UMainMenu* Parent, uint32 Index);

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ServerButton;

	UFUNCTION()
	void OnClicked();

	class UMainMenu* Parent;

	uint32 Index;
};
