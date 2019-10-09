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
	AGameGS();
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = SetAliverUI)
	int LeftAlive = 0;
	int LeftConnect = 0;

	uint8 bIsStart : 1;

	
	void SetAliverUI(int Aliver);
	UFUNCTION()
	void SetAliverUI();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
};
