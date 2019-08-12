// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckPlayerAlive.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API UBTService_CheckPlayerAlive : public UBTService
{
	GENERATED_BODY()

public:
	int a;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
		
	
};
