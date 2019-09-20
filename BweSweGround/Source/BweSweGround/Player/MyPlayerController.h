// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();
	
	virtual void BeginPlay() override;
	
	class UItemToolTipWidgetBase* ItemToolTip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UItemToolTipWidgetBase> ItemToolTipClass;

	void ShowItemName(FString Name);
	void HideItemName();
};
