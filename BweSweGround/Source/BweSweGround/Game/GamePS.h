// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GamePS.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API AGamePS : public APlayerState
{
	GENERATED_BODY()
public:
	AGamePS();

	virtual void BeginPlay() override;
	void SetPlayerNickName();

	UFUNCTION(Server, Reliable)
	void SetNickName(const FString& name);
	void SetNickName_Implementation(const FString& name);

	uint8 SetName : 1;
};
