// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_ChangeSpeed.generated.h"

UENUM(BlueprintType)
enum class EZombieSpeed : uint8
{
	Walk	= 0	UMETA(Display = "Walk"),
	Run		= 1	UMETA(Display = "Run")
};

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API UBTTaskNode_ChangeSpeed : public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EZombieSpeed CurrentSpeed;
};
