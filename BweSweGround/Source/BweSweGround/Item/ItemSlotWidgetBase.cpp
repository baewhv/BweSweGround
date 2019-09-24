// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlotWidgetBase.h"
#include "Item/ItemTable.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Engine/StreamableManager.h"
#include "Engine/Texture2D.h"

void UItemSlotWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	ItemThumbnail = Cast<UBorder>(GetWidgetFromName(TEXT("ItemThumbnail")));
	ItemSlotName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemSlotName")));
	ItemCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCount")));
	ItemButton = Cast<UButton>(GetWidgetFromName(TEXT("ItemButton")));
	if (ItemButton)
	{
		ItemButton->OnClicked.AddDynamic(this, &UItemSlotWidgetBase::ItemUse);
	}
	bUsed = false;
}

void UItemSlotWidgetBase::SetItem(FItemTable ItemData)
{
	CurrentItemData = ItemData;
	FStreamableManager Loader;
	UTexture2D* T_ItemThumbnail = Loader.LoadSynchronous(CurrentItemData.ItemThumnail);
	ItemThumbnail->SetBrushFromTexture(T_ItemThumbnail);
	ItemSlotName->SetText(FText::FromString(CurrentItemData.ItemName));
	ItemCount->SetText(FText::FromString(FString::FromInt(CurrentItemData.ItemCount)));
}

void UItemSlotWidgetBase::ItemUse()
{
	bUsed = false;
	SetVisibility(ESlateVisibility::Collapsed);
}

FReply UItemSlotWidgetBase::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		ItemUse();
	}

	return FReply::Handled();	//하위 위젯에게 더이상 명령을 내리지 않음.
}
