// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGS.h"
#include "UnrealNetwork.h"
#include "Lobby/LobbyPC.h"
#include "Lobby/LobbyWidgetBase.h"
#include "Kismet/GameplayStatics.h"

ALobbyGS::ALobbyGS()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALobbyGS::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->LobbyWidget->SetLeftTime(LeftTime);
	}

}

void ALobbyGS::SetAliver_OnRep()
{
	ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && PC->LobbyWidget)
	{
		//UE_LOG(LogClass, Warning, TEXT("Aliver : %d"), LeftAlive);
			//생성 순서 상 로비 위젯은 꽤 나중.
		PC->LobbyWidget->SetLeftAlive(LeftAlive);
	}
}

void ALobbyGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGS, LeftTime);
	DOREPLIFETIME(ALobbyGS, LeftAlive);	//서버에 보내줘야할 변수는 모조리 적어야 한다.
}
