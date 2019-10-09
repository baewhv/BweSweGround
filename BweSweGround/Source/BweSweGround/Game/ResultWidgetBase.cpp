// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Game/GameGM.h"
#include "Game/GamePC.h"
#include "Kismet/GameplayStatics.h"

void UResultWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	ResultMessageBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("ResultMessageBox")));
	RestartGameButton = Cast<UButton>(GetWidgetFromName(TEXT("RestartGameButton")));
	ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("ExitButton")));

	if (RestartGameButton)
	{
		RestartGameButton->OnClicked.AddDynamic(this, &UResultWidgetBase::RestartGame);
	}
	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UResultWidgetBase::Exit);
	}

}

void UResultWidgetBase::RestartGame()
{
	AGameGM* GM = Cast<AGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->RestartGame();
	}

}

void UResultWidgetBase::Exit()
{
	AGamePC* PC = Cast<AGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if(PC)
	{
		PC->ExitGame();
	}
}
