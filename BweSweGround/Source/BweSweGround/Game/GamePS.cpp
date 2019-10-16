// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePS.h"
#include "MyGameInstance.h"
#include "Player/MyCharacter.h"

AGamePS::AGamePS()
{
	bUseCustomPlayerNames = true;
}

void AGamePS::BeginPlay()
{
	UMyGameInstance* GI = GetGameInstance<UMyGameInstance>();
	if (GI)
	{
		if (GI->NickName.IsEmpty())
		{
			SetPlayerName(GI->NickName);
		}
		UE_LOG(LogClass, Warning, TEXT("my name is %s"), *GetPlayerName());
	}
}

