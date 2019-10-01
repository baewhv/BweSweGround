// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieHPBarWidgetBase.h"
#include "Components/ProgressBar.h"

void UZombieHPBarWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	SetVisibility(ESlateVisibility::Collapsed);

}

void UZombieHPBarWidgetBase::SetZombieHP(float Hp)
{
	if (HPBar)
	{
		if (GetVisibility() == ESlateVisibility::Collapsed)
		{
			
			SetVisibility(ESlateVisibility::Visible);
		}
		HPBar->SetPercent(Hp);
	}
}
