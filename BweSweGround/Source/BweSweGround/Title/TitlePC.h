// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitlePC.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API ATitlePC : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UTitleWidgetBase* TitleWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UTitleWidgetBase> TitleWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USignUpWidgetBase* SignUpWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class USignUpWidgetBase> SignUpWidgetClass;

	void TitleToSignUpWidget();
	void SignUpToTitleWidget();
};
