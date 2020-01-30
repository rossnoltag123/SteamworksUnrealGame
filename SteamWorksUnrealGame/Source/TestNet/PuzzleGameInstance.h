// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"
#include "OnlineSubsystem.h"
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
		void Host() override;

		UFUNCTION(Exec)
		void Join(const FString& Address) override;

		virtual void LoadMainMenu() override;


private:
	TSubclassOf<class UUserWidget> MainMenu_ClassProperty;
	TSubclassOf<class UUserWidget> InGameOverlay_ClassProperty;

	class UMainMenu* Menu;
	class UInGameOverlay* Overlay;
	const FString GetHost(UObject* WorldContextObject);

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	//Call backs for delegates...
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);

	void ServerTravel();
	void CreateSession();
	void DestroySession();
	void SessionFound();
};
