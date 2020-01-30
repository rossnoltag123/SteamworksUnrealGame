// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingCube.generated.h"

/**
 * 
 */
UCLASS()
class TESTNET_API AMovingCube : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	AMovingCube();

	virtual void Tick(float DeltaTime)override;//Sus
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float Speed = 20;

	UPROPERTY(EditAnywhere,Meta = (MakeEditWidget = true))
	FVector TargetLocation;

	void AddActiveTrigger();
	void RemoveActiveTrigger();

private:
	FVector GlobalStartLocation;
	FVector GlobalTargetLocation;

	UPROPERTY(EditAnywhere)
		int ActiveTrigger = 1;

};
