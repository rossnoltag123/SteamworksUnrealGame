// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "Components/TextBlock.h"

#include "MainMenu.generated.h"

/**
 * This class is a child class of UUserWidget ": public UUserWidget, public IMenuInterface"
 * parent class wont display ": public UUserWidget, public IMenuInterface"
 */

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUsername;
};

UCLASS()//, public IMenuInterface
class TESTNET_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer & ObjectInitializer);

	void SetServerList(TArray<FServerData>ServerNames);

	void SelectIndex(uint32 Index);

protected:
	virtual bool Initialize();

private:
	TSubclassOf<class UUserWidget> BP_Servers_ClassProperty;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* RefreshButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
    class UButton* HostMenu_HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostMenu_BackButton;
         
	//Widgets
	UPROPERTY(meta = (BindWiget))
	class UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWiget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWiget))
	class UWidget* HostMenu1;

	UPROPERTY(meta = (BindWiget))
	class UWidget* MainMenu;

	//Other
	UPROPERTY(meta = (BindWiget))
	class UTextBlock* Textblock_ServerName;

	UPROPERTY(meta = (BindWiget))
	class UTextBlock* ServerNameText;

	//UPROPERTY(meta = (BindWiget))
	//class UWidget* HostMenu_TextBox;
	
public:

	UPROPERTY(meta = (BindWiget))
	class UPanelWidget* ScrollBox_Servers;

private:

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenHostMenu();

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

	UFUNCTION()
	void UpdateChilderen();
};