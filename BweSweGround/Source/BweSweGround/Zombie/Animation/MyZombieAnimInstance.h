// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "Zombie/MyZombie.h"

#include "MyZombieAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API UMyZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EZombieState CurrentState = EZombieState::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsStealthKilled : 1;
};
