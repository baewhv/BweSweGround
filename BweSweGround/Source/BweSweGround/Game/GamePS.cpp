// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePS.h"
#include "MyGameInstance.h"
#include "Player/MyCharacter.h"

AGamePS::AGamePS()
{
	SetName = false;
}

void AGamePS::BeginPlay()
{

	Super::BeginPlay();
	//UE_LOG(LogClass, Warning, TEXT("GamePS On! : %s"), *GetName());
	UMyGameInstance* GI = GetGameInstance<UMyGameInstance>();
	if (GI && HasAuthority())
	{
		SetPlayerName(GI->NickName);
		UE_LOG(LogClass, Warning, TEXT("GameName Set! : %s / %s"), *GetName(), *GetPlayerName());
	}
	else
	{
		//UE_LOG(LogClass, Warning, TEXT("GI : %s, HasAuthority : %s"), GI ? *GI->GetName() : TEXT("GI null"), HasAuthority() ? TEXT("HasAuthority"): TEXT("NotHasAutohrity"));
	}
}

void AGamePS::SetPlayerNickName()
{
	UMyGameInstance* GI = GetGameInstance<UMyGameInstance>();
	if (GI)
	{
		SetPlayerName(GI->NickName);
		SetNickName(GI->NickName);
		UE_LOG(LogClass, Warning, TEXT("GameName Set! : %s / %s"), *GetName(), *GetPlayerName());
	}
}

void AGamePS::SetNickName_Implementation(const FString& name)
{
	SetPlayerName(name);
}

