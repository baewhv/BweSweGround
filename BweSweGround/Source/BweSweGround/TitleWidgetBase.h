// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API UTitleWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UEditableTextBox* UserID;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UEditableTextBox* UserPW;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UButton* ConnectButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UButton* MakeServerButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UEditableTextBox* ServerIP;

	UFUNCTION(BlueprintCallable)
	void Connect();
	UFUNCTION(BlueprintCallable)
	void MakeServer();
};

