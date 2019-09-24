// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Item/ItemTable.h"

#include "ItemSlotWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API UItemSlotWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UBorder* ItemThumbnail;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UTextBlock* ItemSlotName;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UTextBlock* ItemCount;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UButton* ItemButton;


	void SetItem(FItemTable ItemData);

	UFUNCTION()
	void ItemUse();

	FItemTable CurrentItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bUsed : 1;

	//NativeOnMouseEvent -> 입력한 버튼을 세부적으로 설정가능. 기본은 모든 클릭이 전부 마우스 좌클릭.
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

};
