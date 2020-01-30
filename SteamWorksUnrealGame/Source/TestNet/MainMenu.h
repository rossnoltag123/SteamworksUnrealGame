// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"


/**
 * This class is a child class of UUserWidget ": public UUserWidget, public IMenuInterface"
 * parent class wont display ": public UUserWidget, public IMenuInterface"
 */

UCLASS()//, public IMenuInterface
class TESTNET_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize();

private:
	//Main menu buttons
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	//Join menu buttons
	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
    class UButton* JoinButton;

	//Widgets
	UPROPERTY(meta = (BindWiget))
	class UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWiget))
	class  UWidget* JoinMenu;

	UPROPERTY(meta = (BindWiget))
	class  UWidget* MainMenu;

	UPROPERTY(meta = (BindWiget))
	class  UEditableTextBox* IPAddressField;

	//Functions
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void QuitGame();
};
