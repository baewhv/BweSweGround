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
	class UGameWidgetBase* GameWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameWidgetBase> GameWidgetClass;

	void ShowGameWidget();
	void HideGameWidget();

	void ShowItemName(FString Name);
	void HideItemName();

	void SetHPBar(float hp);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInventoryWidgetBase* InventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UInventoryWidgetBase> InventoryWidgetClass;

	void ShowInventory();
	void HideInventory();

	virtual void SetupInputComponent() override;

	void InventoryToggle();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UResultWidgetBase* ResultWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UResultWidgetBase> ResultWidgetClass;

	void ShowResult(uint8 result);
	
	void ExitGame();

	//UPROPERTY(VisibleAnywhere, Replicated)
	uint8 bIsCurrentPlayerDie : 1;
};
