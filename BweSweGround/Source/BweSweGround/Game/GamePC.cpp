// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePC.h"
#include "Player/MyPlayerCameraManager.h"
#include "Item/InventoryWidgetBase.h"
#include "Game/GameWidgetBase.h"


AGamePC::AGamePC()
{
	PlayerCameraManagerClass = AMyPlayerCameraManager::StaticClass();
}

void AGamePC::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalPlayerController())
	{
		if (GameWidgetClass)
		{
			GameWidget = CreateWidget<UGameWidgetBase>(this, GameWidgetClass);
			GameWidget->AddToViewport();
			//GameWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (InventoryWidgetClass)
		{
			InventoryWidget = CreateWidget<UInventoryWidgetBase>(this, InventoryWidgetClass);
			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void AGamePC::ShowGameWidget()
{
	if (GameWidget)
	{
		GameWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AGamePC::HideGameWidget()
{
	if (GameWidget)
	{
		GameWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AGamePC::ShowItemName(FString Name)
{
	if (GameWidget)
	{
		GameWidget->ShowItemName(Name);
	}
}

void AGamePC::HideItemName()
{
	if (GameWidget)
	{
		GameWidget->HideItemName();
	}
}

void AGamePC::SetHPBar(float hp)
{
	if (GameWidget)
	{
		GameWidget->SetHPBar(hp);
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
