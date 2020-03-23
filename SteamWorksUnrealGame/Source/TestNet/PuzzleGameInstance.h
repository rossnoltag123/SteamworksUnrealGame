// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"
#include "PuzzleGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TESTNET_API UPuzzleGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

//Want other classes to be able to construct this class
public:
		//Dont need to use virtual infront of constructors...?
		UPuzzleGameInstance(const FObjectInitializer & ObjectInitializer);
		//#include "OnlineSessionInterface.h"

		virtual void Init();

		UFUNCTION(BlueprintCallable)
		void LoadMenuWidget();

		UFUNCTION(BlueprintCallable)
		void InGameLoadMenu();

		UFUNCTION(Exec)
		void Host(FString ServerName) override;

		UFUNCTION(Exec)
		void JoinSession(uint32 Index) override;

		virtual void LoadMainMenu() override;

		//Look for this RefreshServerList....
		//If you want to overide a function, you need to
		//mark the function in the base class with virtual
		//Map to correct overriden fucntion at runtime
		virtual void RefreshServerList() override;

		void StartSession();
		//uint16 NumPubConnections = 3;
	
private:
	TSubclassOf<class UUserWidget> MainMenu_ClassProperty;
	TSubclassOf<class UUserWidget> InGameOverlay_ClassProperty;
	TSubclassOf<class UUserWidget> Servers_ClassProperty;

	class UMainMenu* Menu;
	class UServers* ServerName;
	class UInGameOverlay* Overlay;
	const FString GetHost(UObject* WorldContextObject);

	IOnlineSessionPtr SessionInterface;

	//TSharedPtr Revise----------------------------------------------------------------
	TSharedPtr<class FOnlineSessionSearch> FindSession;

	//Call backs for delegates...
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void ServerTravel();

	FString IdealServerName;

	void CreateSession();
	void DestroySession();
	void SearchSession();

};
