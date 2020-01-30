// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TestNetGameMode.h"
#include "TestNetCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATestNetGameMode::ATestNetGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
