// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ChangeSpeed.h"
#include "Zombie/AI/ZombieAIController.h"
#include "Zombie/MyZombie.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "gameframework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTTaskNode_ChangeSpeed::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMyZombie* Zombie = Cast<AMyZombie>(OwnerComp.GetAIOwner()->GetPawn());

	if (!Zombie)
	{
		return EBTNodeResult::Failed;
	}

	if (CurrentSpeed == EZombieSpeed::Walk)
	{
		Zombie->GetCharacterMovement()->MaxWalkSpeed = Zombie->WalkSpeed;
	}
	else if (CurrentSpeed == EZombieSpeed::Run)
	{
		Zombie->GetCharacterMovement()->MaxWalkSpeed = Zombie->RunSpeed;
	}

	return EBTNodeResult::Succeeded;
}