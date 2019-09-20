// Fill out your copyright notice in the Description page of Project Settings.


#include "GameGM.h"
#include "Item/MasterItem.h"
#include "Kismet/GameplayStatics.h"
#include "Item/ItemPoint.h"
#include "Engine/World.h"

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
		UE_LOG(LogClass, Warning, TEXT("Spawn!"));
	}
}
