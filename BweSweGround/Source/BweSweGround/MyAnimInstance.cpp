// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AMyCharacter* Pawn = Cast<AMyCharacter>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		Speed = Pawn->GetCharacterMovement()->Velocity.Size();
		RightValue = Pawn->RightValue;
		ForwardValue = Pawn->ForwardValue;
		bIsSprint = Pawn->bIsSprint;
		bIsAim = Pawn->bIsAim;
		bIsCrouch = Pawn->bIsCrouch;
		bIsFalling = Pawn->GetCharacterMovement()->IsFalling();
		Yaw = CalculateDirection(Pawn->GetCharacterMovement()->Velocity, Pawn->GetActorRotation());
		AimYaw = Pawn->GetAimOffset().Yaw;
		AimPitch = Pawn->GetAimOffset().Pitch;
	}
}

void UMyAnimInstance::AnimNotify_StartMotion(UAnimNotify * Notify)
{
	AMyCharacter* Pawn = Cast<AMyCharacter>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		Pawn->bIsMotion = true;
		UE_LOG(LogClass, Warning, TEXT("Start Motion"));
	}
}

void UMyAnimInstance::AnimNotify_EndMotion(UAnimNotify * Notify)
{
	AMyCharacter* Pawn = Cast<AMyCharacter>(TryGetPawnOwner());
	if (Pawn && Pawn->IsValidLowLevel())
	{
		Pawn->bIsMotion = false;
		UE_LOG(LogClass, Warning, TEXT("End Motion"));
	}
}
