// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemToolTipWidgetBase.h"
#include "Components/TextBlock.h"

void UItemToolTipWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
}

void UItemToolTipWidgetBase::SetItemName(FString Name)
{
	if (ItemName)
	{
		ItemName->SetText(FText::FromString(Name));
	}
}