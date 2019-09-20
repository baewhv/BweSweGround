// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameGM.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API AGameGM : public AGameModeBase
{
	GENERATED_BODY()
public:
	AGameGM();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AMasterItem> ItemClass;
	
};
