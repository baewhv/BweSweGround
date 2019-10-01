// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZombieHPBarWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API UZombieHPBarWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UProgressBar* HPBar;

	void SetZombieHP(float Hp);
};
