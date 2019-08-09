// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MyBTTask_Disapear.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API UMyBTTask_Disapear : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	int a;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
