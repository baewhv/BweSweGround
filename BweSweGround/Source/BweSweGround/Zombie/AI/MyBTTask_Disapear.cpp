// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_Disapear.h"
#include "Zombie/AI/ZombieAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Game/MyGameModeBase.h"
#include "Engine/World.h"

EBTNodeResult::Type UMyBTTask_Disapear::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMyGameModeBase* GM = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	AZombieAIController* AIC = Cast<AZombieAIController>(OwnerComp.GetOwner());
	UE_LOG(LogClass, Warning, TEXT("ZombieDie!"), );
	if (AIC)
	{
		AIC->GetPawn()->Destroy();
		if (GM)
		{
			UE_LOG(LogClass, Warning, TEXT("%d Zombie Left!"), GM->GetTargetCount());
			GM->CheckTargetCount();
		}
		
	}
	return EBTNodeResult::Succeeded;
}
