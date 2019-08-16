// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_ZombieWalkSpeed.h"
#include "Zombie/MyZombie.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimNotifyState_ZombieWalkSpeed::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	AMyZombie* Zombie = Cast<AMyZombie>(MeshComp->GetOwner());
	if (Zombie && Zombie->CurrentState == EZombieState::Normal)
	{
		//Zombie->GetCharacterMovement()->MaxWalkSpeed = 1.0f;
		Zombie->GetCharacterMovement()->MaxWalkSpeed = Zombie->WalkSpeed;

	}
}

void UAnimNotifyState_ZombieWalkSpeed::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UAnimNotifyState_ZombieWalkSpeed::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	AMyZombie* Zombie = Cast<AMyZombie>(MeshComp->GetOwner());
	if (Zombie && Zombie->CurrentState == EZombieState::Normal)
	{
		//Zombie->GetCharacterMovement()->MaxWalkSpeed = Zombie->WalkSpeed;
		Zombie->GetCharacterMovement()->MaxWalkSpeed = 1.0f;
		//UE_LOG(LogClass, Warning, TEXT("SetWalkEnd! Current Speed : %f, Current State : %d"), Zombie->GetCharacterMovement()->MaxWalkSpeed, (int)Zombie->CurrentState);
	}
}
