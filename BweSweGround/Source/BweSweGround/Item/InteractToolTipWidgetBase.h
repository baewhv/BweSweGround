// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractToolTipWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API UInteractToolTipWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UTextBlock* ItemName;

	UFUNCTION(BlueprintCallable)
	void SetItemName(FString Name);
	
};
