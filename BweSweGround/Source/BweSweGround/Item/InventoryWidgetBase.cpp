// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetBase.h"
#include "Components/Border.h"
#include "Components/ScrollBox.h"
#include "Item/ItemSlotWidgetBase.h"

void UInventoryWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	ItemScroll = Cast<UScrollBox>(GetWidgetFromName(TEXT("ItemScroll")));
	CharacterView = Cast<UBorder>(GetWidgetFromName(TEXT("CharacterView")));
	if (ItemScroll)
	{
		for (int i = 0; i < ItemScroll->GetChildrenCount(); ++i)
		{
			UItemSlotWidgetBase* ItemSlotWidget = Cast< UItemSlotWidgetBase>(ItemScroll->GetChildAt(i));
			if (ItemSlotWidget)
			{
				ItemSlots.Add(ItemSlotWidget);
				ItemSlots[i]->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}


int32 UInventoryWidgetBase::GetEmptySlot()
{
	for (int i = 0; i < ItemSlots.Num(); ++i)
	{
		if (ItemSlots[i]->bUsed == false)
		{
			return i;
		}
	}
	return 0;
}

void UInventoryWidgetBase::SetEmptySlot(int32 Index)
{
	if (Index < 0 || Index> ItemSlots.Num())
	{
		return;
	}
	ItemSlots[Index]->bUsed = false;
	ItemSlots[Index]->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventoryWidgetBase::SetItemSlot(int32 Index, FItemTable ItemData)
{
	if (Index < 0 || Index> ItemSlots.Num())
	{
		return;
	}
	ItemSlots[Index]->bUsed = true;
	ItemSlots[Index]->SetItem(ItemData);
	ItemSlots[Index]->SetVisibility(ESlateVisibility::Visible);
}
