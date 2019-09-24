// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePC.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API AGamePC : public APlayerController
{
	GENERATED_BODY()

public:
	AGamePC();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UItemToolTipWidgetBase* ItemToolTip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UItemToolTipWidgetBase> ItemToolTipClass;

	void ShowItemName(FString Name);
	void HideItemName();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInventoryWidgetBase* InventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UInventoryWidgetBase> InventoryWidgetClass;

	void ShowInventory();
	void HideInventory();

	virtual void SetupInputComponent() override;

	void Inventory();
};
