// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "Player/MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Zombie/MyZombie.h"

AMyGameModeBase::AMyGameModeBase()
{
	DefaultPawnClass = AMyCharacter::StaticClass();	

}

int AMyGameModeBase::GetTargetCount()
{
	TArray<AActor *> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyZombie::StaticClass(), OutActors);

	return OutActors.Num();
}

void AMyGameModeBase::CheckTargetCount()
{
	if (GetTargetCount() == 0)
	{
		FString mapName = GetWorld()->GetMapName().Mid(GetWorld()->StreamingLevelsPrefix.Len());
		UE_LOG(LogClass, Warning, TEXT("%s"), *mapName);
		if (mapName == TEXT("Scene01"))
		{
			
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("Scene02"));
		}
		else if (mapName == TEXT("Scene02"))
		{
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("Scene03"));
		}
		else
		{
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("Scene01"));
		}
	}

}