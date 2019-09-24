// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Item/ItemTable.h"

#include "InventoryWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API UInventoryWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;


	//스크롤 박스에 이미 생성된 위젯 리스트
	TArray<class UItemSlotWidgetBase*> ItemSlots;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UScrollBox* ItemScroll;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UBorder* CharacterView;

	//빈 슬롯 찾기.
	UFUNCTION(BlueprintCallable)
	int32 GetEmptySlot();

	//사용중인 슬롯 비우기.
	UFUNCTION(BlueprintCallable)
	void SetEmptySlot(int32 Index);

	//아이템 넣기
	UFUNCTION(BlueprintCallable)
	void SetItemSlot(int32 Index, FItemTable ItemData);
	
};
