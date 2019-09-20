// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyPlayerCameraManager.h"
#include "Item/ItemToolTipWidgetBase.h"


AMyPlayerController::AMyPlayerController()
{
	PlayerCameraManagerClass = AMyPlayerCameraManager::StaticClass();


}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		CreateWidget<UItemToolTipWidgetBase>(this, ItemToolTipClass);
	}
}

void AMyPlayerController::ShowItemName(FString Name)
{
}

void AMyPlayerController::HideItemName()
{
}
