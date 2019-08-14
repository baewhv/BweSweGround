// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ReloadEnd.h"
#include "Player/MyCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_ReloadEnd::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);

	AMyCharacter* Player = Cast<AMyCharacter>(MeshComp->GetOwner());

	if (Player && Player->bIsReloading)
	{
		Player->Reload_End();
	}

}