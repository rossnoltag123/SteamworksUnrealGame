// Fill out your copyright notice in the Description page of Project Settings.

//#include "Engine/World.h"
#include "LobbyGameMode.h"
#include "TimerManager.h"
#include "PuzzleGameInstance.h"


void ALobbyGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumberOfPlayers;

	if (NumberOfPlayers >= 5)
	{
		GetWorldTimerManager().SetTimer(GameStartTimer,this,&ALobbyGameMode::StartGame,10);
	}
}

void ALobbyGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}

void ALobbyGameMode::StartGame()
{
	//Use auto when there is alot of repetition
	auto GameInstance = Cast<UPuzzleGameInstance>(GetGameInstance());

	if (GameInstance == nullptr)return;
	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr))return;

	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/PuzzlePlatform/Maps/MainLevel?listen");
}