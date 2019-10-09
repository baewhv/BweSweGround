// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePC.h"
#include "Player/MyPlayerCameraManager.h"
#include "Item/InventoryWidgetBase.h"
#include "Game/GameWidgetBase.h"
#include "Game/GameGS.h"
#include "Game/ResultWidgetBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


AGamePC::AGamePC()
{
	PlayerCameraManagerClass = AMyPlayerCameraManager::StaticClass();
	bIsCurrentPlayerDie = false;
}

void AGamePC::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameOnly());
	if (IsLocalPlayerController())
	{
		if (GameWidgetClass)
		{
			GameWidget = CreateWidget<UGameWidgetBase>(this, GameWidgetClass);
			GameWidget->AddToViewport();
			AGameGS* GS = Cast<AGameGS>(UGameplayStatics::GetGameState(GetWorld()));
			if (GS)
			{
				GameWidget->SetAliveCount(GS->LeftAlive);//카운트 초기화
			}
			else
			{
				UE_LOG(LogClass, Warning, TEXT("GS 로딩 실패!"));
			}
		}
		if (InventoryWidgetClass)
		{
			InventoryWidget = CreateWidget<UInventoryWidgetBase>(this, InventoryWidgetClass);
			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (ResultWidgetClass)
		{
			ResultWidget = CreateWidget<UResultWidgetBase>(this, ResultWidgetClass);
			ResultWidget->AddToViewport();
			ResultWidget->SetVisibility(ESlateVisibility::Collapsed);
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

	InputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &AGamePC::InventoryToggle);
}

void AGamePC::InventoryToggle()
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

void AGamePC::ShowResult(uint8 result)
{
	if (/*IsLocalPlayerController() &&*/
		ResultWidget &&
		ResultWidget->RestartGameButton &&
		ResultWidget->ResultMessageBox /*&& ResultWidget->GetVisibility() == ESlateVisibility::Collapsed*/)
	{
		//UE_LOG(LogClass, Warning, TEXT("ShowResult at PC - %s"), result ? TEXT("Alive"): TEXT("Die"));
		SetInputMode(FInputModeGameAndUI());
		ResultWidget->SetVisibility(ESlateVisibility::Visible);
		if (result)
		{
			ResultWidget->ResultMessageBox->SetText(FText::FromString(TEXT("승리")));
		}
		else
		{
			ResultWidget->ResultMessageBox->SetText(FText::FromString(TEXT("패배")));
		}
		ResultWidget->RestartGameButton->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}

void AGamePC::ExitGame()
{
	//ClientTravel(TEXT("Title"), ETravelType::TRAVEL_Absolute);	->추후 기능 체크!
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}
