// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "MyPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API AMyPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	virtual void UpdateCamera(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float NormalFOV = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float IronsightFOV = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InterpSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float NormalZ = -88.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CrouchingZ = -40.0f;

	
};
