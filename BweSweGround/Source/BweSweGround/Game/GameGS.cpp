// Fill out your copyright notice in the Description page of Project Settings.


#include "GameGS.h"
#include "GamePC.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealNetwork.h"
#include "Engine/World.h"
#include "GameWidgetBase.h"
#include "Player/MyCharacter.h"
#include "MyGameInstance.h"


AGameGS::AGameGS()
{
	bIsStart = false;
}

void AGameGS::SetAliverUI(int Aliver)
{
	UMyGameInstance* GI = GetGameInstance<UMyGameInstance>();
	if (GI && HasAuthority())
	{
		UE_LOG(LogClass, Warning, TEXT("%s Set Aliver in property! : %d"), *GI->GetUserID(), LeftAlive);
	}
	//LeftAlive = Aliver;
	AGamePC* PC = Cast<AGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && PC->GameWidget && HasAuthority())
	{
		PC->GameWidget->SetAliveCount(Aliver);
	}

}

void AGameGS::SetAliverUI()
{
	UMyGameInstance* GI = GetGameInstance<UMyGameInstance>();
	if (GI)
	{
		UE_LOG(LogClass, Warning, TEXT("%s Set Aliver! : %d"), *GI->GetUserID(), LeftAlive);
	}
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