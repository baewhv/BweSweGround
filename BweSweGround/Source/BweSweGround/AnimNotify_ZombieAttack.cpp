// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ZombieAttack.h"
#include "Zombie/MyZombie.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_ZombieAttack::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);

	AMyZombie* Zombie = Cast<AMyZombie>(MeshComp->GetOwner());

	if (Zombie)
	{
		Zombie->Attack();
	}

}
