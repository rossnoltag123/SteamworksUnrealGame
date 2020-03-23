// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Button.h"
#include "Servers.h"
#include "MainMenu.h"

void UServers::Setup(class UMainMenu* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
	ServerButton->OnClicked.AddDynamic(this, &UServers::OnClicked);
}

void UServers::OnClicked()
{
	Parent->SelectIndex(Index);
}