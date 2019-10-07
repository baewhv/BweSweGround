// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePC.h"
#include "Title/SignUpWidgetBase.h"
#include "Title/TitleWidgetBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


void ATitlePC::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalPlayerController())
	{
		if (TitleWidgetClass)
		{
			TitleWidget = CreateWidget<UTitleWidgetBase>(this, TitleWidgetClass);
			TitleWidget->AddToViewport();
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this, Cast<UWidget>(TitleWidget));
			//GameWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (SignUpWidgetClass)
		{
			SignUpWidget = CreateWidget<USignUpWidgetBase>(this, SignUpWidgetClass);
			SignUpWidget->AddToViewport();
			SignUpWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	bShowMouseCursor = true;
}

void ATitlePC::TitleToSignUpWidget()
{
	if (SignUpWidget && SignUpWidget->GetVisibility() == ESlateVisibility::Collapsed)
	{
		SignUpWidget->SetVisibility(ESlateVisibility::Visible);
		TitleWidget->SetVisibility(ESlateVisibility::Collapsed);
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this, Cast<UWidget>(SignUpWidget));
	}
}

void ATitlePC::SignUpToTitleWidget()
{
	if (TitleWidget && TitleWidget->GetVisibility() == ESlateVisibility::Collapsed)
	{
		TitleWidget->SetVisibility(ESlateVisibility::Visible);
		SignUpWidget->SetVisibility(ESlateVisibility::Collapsed);
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this, Cast<UWidget>(TitleWidget));
	}
}
