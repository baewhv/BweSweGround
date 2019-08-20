// Fill out your copyright notice in the Description page of Project Settings.


#include "MyZombieAnimInstance.h"
#include "Zombie/MyZombie.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMyZombieAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMyZombie* Pawn = Cast<AMyZombie>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		CurrentState = Pawn->CurrentState;
		Speed = Pawn->GetCharacterMovement()->Velocity.Size();
		bIsStealthKilled = Pawn->bIsStealthKilled;
	}
}

