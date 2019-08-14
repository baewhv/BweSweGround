// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_ZombieAttacking.h"
#include "Zombie/MyZombie.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_ZombieAttacking::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	AMyZombie* Zombie = Cast<AMyZombie>(MeshComp->GetOwner());
	if(Zombie)
	{
		Zombie->bIsAttack = true;
	}
}

void UAnimNotifyState_ZombieAttacking::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyBegin(MeshComp, Animation, FrameDeltaTime);
}

void UAnimNotifyState_ZombieAttacking::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	
	AMyZombie* Zombie = Cast<AMyZombie>(MeshComp->GetOwner());
	if (Zombie)
	{
		Zombie->bIsAttack = false;
	}
	Super::NotifyEnd(MeshComp, Animation);
}
