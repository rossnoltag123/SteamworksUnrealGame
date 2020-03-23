// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestNetGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TESTNET_API ALobbyGameMode : public ATestNetGameMode
{
	GENERATED_BODY()

public:

	void PostLogin(APlayerController * NewPlayer)override;
	void Logout(AController * NewPlayer)override;

private:

	void StartGame();

	uint32 NumberOfPlayers = 0;
	FTimerHandle GameStartTimer;

};
