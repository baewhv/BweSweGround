// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_Disapear.h"
#include "Zombie/AI/ZombieAIController.h"

EBTNodeResult::Type UMyBTTask_Disapear::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AZombieAIController* AIC = Cast<AZombieAIController>(OwnerComp.GetOwner());
	if (AIC)
	{
		AIC->GetPawn()->Destroy();
	}

	return EBTNodeResult::Succeeded;
}
