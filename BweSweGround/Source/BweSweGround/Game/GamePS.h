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
	virtual void SetPlayerName(const FString& S) override;
	
};
