// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGM.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Lobby/LobbyGS.h"

void ALobbyGM::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(LeftTimerHandle, this, &ALobbyGM::LeftTimer, 1.0f, true, 1.0f);
}

void ALobbyGM::LeftTimer()
{
	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		GS->LeftTime--;
	}
}

void ALobbyGM::PostLogin(APlayerController * NewPlayer)
{
	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		GS->LeftAlive++;
		GS->SetAliver();
	}
}