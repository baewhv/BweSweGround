// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_StealthKillEnd.h"
#include "Player/MyCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_StealthKillEnd::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	UE_LOG(LogClass, Warning, TEXT("EndStelthKill Called!"));
	AMyCharacter* Player = Cast<AMyCharacter>(MeshComp->GetOwner());

	if (Player && Player->bIsStealthKill)
	{
		Player->EndStealthKill();
	}

}