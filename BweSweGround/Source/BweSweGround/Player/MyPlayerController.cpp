// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyPlayerCameraManager.h"
#include "Item/ItemToolTipWidgetBase.h"
#include "Item/InventoryWidgetBase.h"


AMyPlayerController::AMyPlayerController()
{
	PlayerCameraManagerClass = AMyPlayerCameraManager::StaticClass();


}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		if (ItemToolTipClass)
		{
			ItemToolTip = CreateWidget<UItemToolTipWidgetBase>(this, ItemToolTipClass);
			ItemToolTip->AddToViewport();
			ItemToolTip->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (ItemToolTipClass)
		{
			InventoryWidget = CreateWidget<UInventoryWidgetBase>(this, InventoryWidgetClass);
			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		}

	}
}

void AMyPlayerController::ShowItemName(FString Name)
{
	if (ItemToolTip)
	{
		ItemToolTip->SetItemName(Name);
		ItemToolTip->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMyPlayerController::HideItemName()
{
	if (ItemToolTip)
	{
		ItemToolTip->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMyPlayerController::ShowInventory()
{
	InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	bShowMouseCursor = true;
	SetInputMode(FInputModeGameAndUI());
}

void AMyPlayerController::HideInventory()
{
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UE_LOG(LogClass, Warning, TEXT("Binding"));
	InputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &AMyPlayerController::Inventory);
}

void AMyPlayerController::Inventory()
{
	if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		HideInventory();
	}
	else
	{
		ShowInventory();
	}
}
