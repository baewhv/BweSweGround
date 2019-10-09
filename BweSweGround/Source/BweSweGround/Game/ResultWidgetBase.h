// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API UResultWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UTextBlock* ResultMessageBox;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UButton* RestartGameButton;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UButton* ExitButton;
	
	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void Exit();

};
