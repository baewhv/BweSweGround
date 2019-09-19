// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPC.h"
#include "LobbyWidgetBase.h"
#include "Components/Button.h"

void ALobbyPC::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{

		//#include "Path", 경로, UE4 Asset, BP 
		FSoftClassPath LobbyWidgetClass_BP_PATH(TEXT("WidgetBlueprint'/Game/Blueprints/UI/Lobby/LobbyWidget.LobbyWidget_C'"));

		//위의 경로로 호출
		UClass* LobbyWidgetClass_BP = LobbyWidgetClass_BP_PATH.TryLoadClass<ULobbyWidgetBase>();

		if (LobbyWidgetClass_BP)
		{
			LobbyWidget = Cast<ULobbyWidgetBase>(NewObject<ULobbyWidgetBase>(this, LobbyWidgetClass_BP));
			//LobbyWidget = CreateWidget<ULobbyWidgetBase>(this, LobbyWidgetClass);
			if (LobbyWidget)
			{
				LobbyWidget->AddToViewport();
				bShowMouseCursor = true;
				//FInputModeGameAndUI f;
				//f. ->이후 옵션 설정.
				SetInputMode(FInputModeGameAndUI());

				if (!HasAuthority())
				{
					LobbyWidget->StartGameButton->SetVisibility(ESlateVisibility::Collapsed);
					
				}
				else
				{
					LobbyWidget->PlayStartButton();
				}

			}
		}
	}
}


bool ALobbyPC::C2S_SendMessage_Validate(const FText & Message)
{
	return true;
}

void ALobbyPC::C2S_SendMessage_Implementation(const FText & Message)
{
	

	for (auto iter = GetWorld()->GetPlayerControllerIterator(); iter; ++iter)
	{
		ALobbyPC* PC = Cast<ALobbyPC>(*iter);
		if (PC)
		{
			UE_LOG(LogClass, Warning, TEXT("C2S_SendMessage_Implementation"));
			PC->S2C_SendMessage(Message);
		}
	}
}

void ALobbyPC::S2C_SendMessage_Implementation(const FText & Message)
{
	if (LobbyWidget)
	{
		LobbyWidget->AddMessage(Message);
	}
}
