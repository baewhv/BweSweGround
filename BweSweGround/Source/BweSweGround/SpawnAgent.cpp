// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnAgent.h"
#include "Engine/World.h"
#include "Enemy.h"

// Sets default values
ASpawnAgent::ASpawnAgent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnAgent::BeginPlay()
{
	Super::BeginPlay();
	AEnemy* NewPawn = GetWorld()->SpawnActor<AEnemy>(SpawnEnemy, GetActorTransform());
	if (NewPawn)
	{
		NewPawn->getSpawnPoint(this);
	}
}

// Called every frame
void ASpawnAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

