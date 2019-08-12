// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetSpeed.h"
#include "Zombie/AI/ZombieAIController.h"
#include "Zombie/MyZombie.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_SetSpeed::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMyZombie* Zombie = Cast<AMyZombie>(OwnerComp.GetAIOwner()->GetPawn());

	if (!Zombie)
	{
		return EBTNodeResult::Failed;
	}
	//GetCharacterMovement()->MaxWalkSpeed
	switch (Zombie->CurrentState)
	{
		case EZombieState::Normal:
			Zombie->Sprint_End();
			break;
		case EZombieState::Chase:
			Zombie->Sprint_Start();
			break;
	}


	return EBTNodeResult::Succeeded;
}
