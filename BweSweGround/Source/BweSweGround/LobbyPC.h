// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPC.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API ALobbyPC : public APlayerController
{
	GENERATED_BODY()
public:
	int a;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class UUserWidget> LobbyWidgetClass;

	virtual void BeginPlay() override;
	
};
