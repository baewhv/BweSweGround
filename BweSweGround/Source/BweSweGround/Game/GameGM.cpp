// Fill out your copyright notice in the Description page of Project Settings.


#include "GameGM.h"
#include "Item/MasterItem.h"
#include "Kismet/GameplayStatics.h"
#include "Item/ItemPoint.h"
#include "Engine/World.h"
#include "GameGS.h"
#include "Player/MyCharacter.h"
#include "GamePC.h"

AGameGM::AGameGM()
{
}

void AGameGM::BeginPlay()
{
	Super::BeginPlay();
	
	//맵에서 아이템 생성 위치 가져오기
	TArray<AActor*> ItemLocations;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemPoint::StaticClass(), ItemLocations);

	//아이템 생성
	for (auto Location : ItemLocations) 
	{
		GetWorld()->SpawnActor<AMasterItem>(ItemClass, Location->GetTransform());
	}
}
void AGameGM::checkAliver()
{
	//UE_LOG(LogClass, Warning, TEXT("Check!"));
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Player"), Players);

	uint32 AlivePlayer = 0;
	//살아있는 인원 체크.
	for (auto Player : Players)
	{
		if (Player)
		{
			AMyCharacter* Character = Cast<AMyCharacter>(Player);
			if (Character->bIsAlive)
			{
				AlivePlayer++;
			}
		}
	}
	AGameGS* GS = GetGameState<AGameGS>();
	if (GS)
	{
		GS->SetAliverUI(AlivePlayer);
		if (Players.Num() > 1)
		{
			GS->bIsStart = true;
		}
		if (GS->bIsStart && AlivePlayer <= 1)
		{
			
			UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Player"), Players);


			for (auto Player : Players)
			{
				AMyCharacter* Character = Cast<AMyCharacter>(Player);
				if (Character)
				{
					Character->SetEndUI();
					Character->bIsGameDone = true;
				}

			}
		}
	}
}
void AGameGM::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	checkAliver();	//인원이 2명 이상일 경우 시작 체크.
	AGameGS* GS = GetGameState<AGameGS>();
	if(GS && HasAuthority())
	{
		GS->LeftAlive++;
		GS->LeftConnect++;
		UE_LOG(LogClass, Warning, TEXT("%d"), GS->LeftAlive);
	}
}

void AGameGM::Logout(AController * Exiting)
{
	Super::Logout(Exiting);
	checkAliver();	//인원이 2명 이상일 경우 시작 체크.
	AGameGS* GS = GetGameState<AGameGS>();
	if (GS)
	{
		GS->LeftConnect--;
		AGamePC* PC = Cast<AGamePC>(Exiting);
		if (PC && !PC->bIsCurrentPlayerDie)
		{
			UE_LOG(LogClass, Warning, TEXT("Out"));
			GS->LeftAlive--;
		}
	}
}

void AGameGM::RestartGame()
{
	GetWorld()->ServerTravel(TEXT("Lobby"));
}
