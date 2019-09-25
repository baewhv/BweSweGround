// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameGS.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API AGameGS : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(Replicated, VisibleAnywhere)
	int LeftAlive = 0;

	UFUNCTION()
	void SetAliver_OnRep();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
};
