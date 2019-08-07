// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RightValue;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ForwardValue;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Yaw;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsSprint : 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsAim : 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsCrouch : 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsFalling : 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsAlive : 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FireAlpha;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AimYaw;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AimPitch;

	UFUNCTION()
	void AnimNotify_StartMotion(UAnimNotify* Notify);

	UFUNCTION()
	void AnimNotify_EndMotion(UAnimNotify* Notify);




};
