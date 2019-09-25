// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API UGameWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UTextBlock* KillCount;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UTextBlock* AliveCount;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UTextBlock* DegreeCount;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UTextBlock* Magazine;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UTextBlock* ArmedBullet;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UScrollBox* KillLogBox;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UScrollBar* CurrHP;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UItemToolTipWidgetBase* NameToolTip;

	UFUNCTION(BlueprintCallable)
		void SetAliveCount(int32 LeftAlive);
	UFUNCTION(BlueprintCallable)
		void SetKillCount(int32 KillCounting);
	UFUNCTION(BlueprintCallable)
		void SetLocationDegree(int32 Degree);
	UFUNCTION(BlueprintCallable)
		void SetArmedBullet(int32 bullet);
	UFUNCTION(BlueprintCallable)
		void SetMagazine(int32 bullet);
	UFUNCTION(BlueprintCallable)
		void SetKillLog(const FText& Text);
	
};
