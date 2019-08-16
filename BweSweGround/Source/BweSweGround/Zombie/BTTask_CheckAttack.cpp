// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckAttack.h"
#include "Zombie/MyZombie.h"
#include "Zombie/AI/ZombieAIController.h"

EBTNodeResult::Type UBTTask_CheckAttack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMyZombie* Zombie = Cast<AMyZombie>(OwnerComp.GetAIOwner()->GetPawn());

	if (!Zombie)
	{
		return EBTNodeResult::Failed;
	}

	if (Zombie->bIsAttack)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
