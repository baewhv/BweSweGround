// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePC.h"
#include "Player/MyPlayerCameraManager.h"
#include "Item/ItemToolTipWidgetBase.h"
#include "Item/InventoryWidgetBase.h"


AGamePC::AGamePC()
{
	PlayerCameraManagerClass = AMyPlayerCameraManager::StaticClass();
}

void AGamePC::BeginPlay()
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
		if (InventoryWidgetClass)
		{
			InventoryWidget = CreateWidget<UInventoryWidgetBase>(this, InventoryWidgetClass);
			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void AGamePC::ShowItemName(FString Name)
{
	if (ItemToolTip)
	{
		ItemToolTip->SetItemName(Name);
		ItemToolTip->SetVisibility(ESlateVisibility::Visible);
	}
}

void AGamePC::HideItemName()
{
	if (ItemToolTip)
	{
		ItemToolTip->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AGamePC::ShowInventory()
{
	InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	bShowMouseCursor = true;
	SetInputMode(FInputModeGameAndUI());
}

void AGamePC::HideInventory()
{
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
}

void AGamePC::SetupInputComponent()
{
	Super::SetupInputComponent();

	UE_LOG(LogClass, Warning, TEXT("Binding"));
	InputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &AGamePC::Inventory);
}

void AGamePC::Inventory()
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
