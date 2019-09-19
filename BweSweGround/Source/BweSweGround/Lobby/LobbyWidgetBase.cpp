// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyPC.h"
#include "MyGameInstance.h"
#include "LobbyGS.h"
#include "LobbyGM.h"

void ULobbyWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	AliveCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("AliveCount")));
	PublicMessageBox = Cast<UTextBlock>(GetWidgetFromName(TEXT("PublicMessageBox")));
	ChattingBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ChattingBox")));
	ChatBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ChatBox")));
	StartGameButton = Cast<UButton>(GetWidgetFromName(TEXT("StartGameButton")));

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &ULobbyWidgetBase::StartGame);
	}
	if (ChatBox)
	{
		ChatBox->OnTextCommitted.AddDynamic(this, &ULobbyWidgetBase::OnTextCommit);
	}
	/*ALobbyGS* GS = GetWorld()->GetGameState<ALobbyGS>();
	if (GS)
	{
		GS->SetAliver_OnRep();
	}*/
	
}



void ULobbyWidgetBase::StartGame()
{
	UE_LOG(LogClass, Warning, TEXT("Start!"));
	
	ALobbyGM* GM = Cast<ALobbyGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->StartGame();
	}
}

void ULobbyWidgetBase::OnTextCommit(const FText & Text, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
	case ETextCommit::Type::OnCleared:
		{
			//메시지 전송 후 	
			ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if (PC)
			{
				ChatBox->SetUserFocus(PC);
			}
		}
		break;
		case ETextCommit::Type::OnEnter:
		{
			//메시지 전송
			if (!Text.IsEmpty())
			{
				//메시지 전송
				ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
				UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
				if (PC && GI) 
				{
					FString UserID = GI->GetUserID();
					FString Temp = FString::Printf(TEXT("%s : %s"), *UserID, *(Text.ToString()));
					PC->C2S_SendMessage(FText::FromString(Temp));
					ChatBox->SetText(FText::FromString(TEXT("")));
				}
			}
			else
			{
				//채팅 끝
			}
		}
		break;
		case ETextCommit::Type::OnUserMovedFocus:
		{

		}
		break;
	}

	//PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" }); build.cs의 주석 해제.

}

void ULobbyWidgetBase::AddMessage(const FText & Text)
{
	if (ChattingBox)
	{
		UTextBlock* NewBlock = NewObject<UTextBlock>(ChattingBox);
		if (NewBlock)
		{
			NewBlock->SetText(Text);
			NewBlock->Font.Size = 18.0f;
			ChattingBox->AddChild(NewBlock);
			ChattingBox->ScrollToEnd();	//채팅창 위치 리셋.
		}
	}
}

void ULobbyWidgetBase::SetLeftTime(int32 LeftTime)
{
	if (PublicMessageBox)
	{
		FString Temp = FString::Printf(TEXT("%d초 남았습니다."), LeftTime);
		PublicMessageBox->SetText(FText::FromString(*Temp));
	}
}

void ULobbyWidgetBase::SetLeftAlive(int32 LeftAlive)
{
	if (AliveCount)
	{
		FString Temp = FString::Printf(TEXT("%d Alive"), LeftAlive);
		AliveCount->SetText(FText::FromString(*Temp));
	}
}

void ULobbyWidgetBase::PlayStartButton_Implementation()
{
	
}

