// Fill out your copyright notice in the Description page of Project Settings.


#include "GameGS.h"
#include "GamePC.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealNetwork.h"
#include "Engine/World.h"
#include "GameWidgetBase.h"


void AGameGS::SetAliver_OnRep()
{
	AGamePC* PC = Cast<AGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && PC->GameWidget)
	{
		PC->GameWidget->SetAliveCount(LeftAlive);
	}
}

void AGameGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGameGS, LeftAlive);	//서버에 보내줘야할 변수는 모조리 적어야 한다.
}