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

public:
	UMainMenu(const FObjectInitializer & ObjectInitializer);

	void SetServerList(TArray<FString>ServerNames);

	void SelectIndex(uint32 Index);

protected:
	virtual bool Initialize();

private:
	TSubclassOf<class UUserWidget> BP_Servers_ClassProperty;
	
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

	UPROPERTY(meta = (BindWidget))
	class UButton* RefreshButton;
	         
	//Widgets
	UPROPERTY(meta = (BindWiget))
	class UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWiget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWiget))
	class UWidget* MainMenu;

	//Other
	UPROPERTY(meta = (BindWiget))
	class UTextBlock* Textblock_ServerName;

public:
	UPROPERTY(meta = (BindWiget))
	class UPanelWidget* ScrollBox_Servers;

private:
	UPROPERTY(meta = (BindWiget))
	class UPanelWidget* IPAddressField;

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

	UFUNCTION()
	void RefreshServerList();

	const FString GetHostAddress(UObject* WorldContextObject);

	TOptional<uint32> SelectedIndex;
};