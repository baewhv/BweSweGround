// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBar.h"
#include "Item/ItemToolTipWidgetBase.h"

void UGameWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	KillCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("KillCount")));
	AliveCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("AliveCount")));
	DegreeCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("DegreeCount")));
	Magazine = Cast<UTextBlock>(GetWidgetFromName(TEXT("TotalBullet")));
	ArmedBullet = Cast<UTextBlock>(GetWidgetFromName(TEXT("ArmedBullet")));
	KillLogBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("KillLogBox")));
	CurrHP = Cast<UScrollBar>(GetWidgetFromName(TEXT("CurrHP")));
	NameToolTip = Cast<UItemToolTipWidgetBase>(GetWidgetFromName(TEXT("ChattingBox")));
}

void UGameWidgetBase::SetAliveCount(int32 LeftAlive)
{
	if (AliveCount)
	{
		FString Temp = FString::Printf(TEXT("%d Alive"), LeftAlive);
		AliveCount->SetText(FText::FromString(*Temp));
	}
}

void UGameWidgetBase::SetKillCount(int32 KillCounting)
{
	if (KillCount)
	{
		FString Temp = FString::Printf(TEXT("%d Kill"), KillCounting);
		KillCount->SetText(FText::FromString(*Temp));
	}
}

void UGameWidgetBase::SetLocationDegree(int32 Degree)
{
	if (DegreeCount)
	{
		FString Temp = FString::Printf(TEXT("%d˚"), Degree);
		DegreeCount->SetText(FText::FromString(*Temp));
	}
}

void UGameWidgetBase::SetArmedBullet(int32 value)
{
	if (ArmedBullet)
	{
		FString Temp = FString::Printf(TEXT("%d"), value);
		ArmedBullet->SetText(FText::FromString(*Temp));
	}
}

void UGameWidgetBase::SetMagazine(int32 value)
{
	if (Magazine)
	{
		FString Temp = FString::Printf(TEXT("%d"), value);
		Magazine->SetText(FText::FromString(*Temp));
	}
}

void UGameWidgetBase::SetKillLog(const FText & Text)
{
	if (KillLogBox)
	{
		UTextBlock* NewBlock = NewObject<UTextBlock>(KillLogBox);
		if (NewBlock)
		{
			NewBlock->SetText(Text);
			NewBlock->Font.Size = 18.0f;
			KillLogBox->AddChild(NewBlock);
			KillLogBox->ScrollToEnd();	//채팅창 위치 리셋.
		}
	}
}
