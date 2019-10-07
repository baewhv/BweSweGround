// Fill out your copyright notice in the Description page of Project Settings.


#include "GameGM.h"
#include "Item/MasterItem.h"
#include "Kismet/GameplayStatics.h"
#include "Item/ItemPoint.h"
#include "Engine/World.h"
#include "GameGS.h"

AGameGM::AGameGM()
{
}

void AGameGM::BeginPlay()
{
	Super::BeginPlay();
	bIsStart = false;
	
	//맵에서 아이템 생성 위치 가져오기
	TArray<AActor*> ItemLocations;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemPoint::StaticClass(), ItemLocations);

	//아이템 생성
	for (auto Location : ItemLocations) 
	{
		GetWorld()->SpawnActor<AMasterItem>(ItemClass, Location->GetTransform());
	}
}
void AGameGM::checkAliver(int LeftAlive)
{
	if (LeftAlive > 1)
	{
		bIsStart = true;
	}
	if (bIsStart && LeftAlive <= 1)
	{

	}
}
void AGameGM::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	AGameGS* GS = GetGameState<AGameGS>();
	if (GS)
	{
		GS->LeftAlive++;
		if (HasAuthority())
		{
			GS->SetAliver_OnRep();
		}
		if (!bIsStart)
		{
			checkAliver(GS->LeftAlive);	//인원이 2명 이상일 경우 시작 체크.
		}
	}
}

void AGameGM::Logout(AController * Exiting)
{
	Super::Logout(Exiting);
	AGameGS* GS = GetGameState<AGameGS>();
	if (GS)
	{
		GS->LeftAlive--;
		if (HasAuthority())//권한이 있는지 체크.
		{
			GS->SetAliver_OnRep();
			if (bIsStart)
			{
				checkAliver(GS->LeftAlive);
			}
		}
	}
}