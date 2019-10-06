// Fill out your copyright notice in the Description page of Project Settings.


#include "SignUpWidgetBase.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Title/TitlePC.h"
#include "Kismet/GameplayStatics.h"
#include "Http.h"
#include "Json.h"
#include "MyGameInstance.h"


void USignUpWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	bIsFocusable = true;
	NickName = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("UserID")));
	UserPW = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("UserPW")));
	CheckPW = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("CheckPW")));
	UserNick = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("UserNick")));
	SignUpButton = Cast<UButton>(GetWidgetFromName(TEXT("SignUpButton")));
	if (SignUpButton)
	{
		SignUpButton->OnClicked.AddDynamic(this, &USignUpWidgetBase::SignUp);
	}
	BackButton = Cast<UButton>(GetWidgetFromName(TEXT("BackButton")));
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &USignUpWidgetBase::BackToTitle);
	}
	ToolTip = Cast<UTextBlock>(GetWidgetFromName(TEXT("ToolTip")));
}

void USignUpWidgetBase::BackToTitle()
{
	UE_LOG(LogClass, Warning, TEXT("Enter"));
	ATitlePC* PC = Cast<ATitlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		UE_LOG(LogClass, Warning, TEXT("Active"));
		PC->SignUpToTitleWidget();
	}
}

void USignUpWidgetBase::SignUp()
{
	if (UserNick && ToolTip && UserPW && CheckPW)
	{
		FString Temp;
		if (NickName->GetText().ToString().Len() < 1)
		{
			Temp = FString::Printf(TEXT("ID를 입력해주세요."));
			ToolTip->SetText(FText::FromString(*Temp));
			return;
		}
		else if (UserPW->GetText().ToString().Len() < 1)
		{
			Temp = FString::Printf(TEXT("비밀번호를 입력해주세요."));
			ToolTip->SetText(FText::FromString(*Temp));
			return;
		}
		else if (CheckPW->GetText().ToString().Len() < 1)
		{
			Temp = FString::Printf(TEXT("비밀번호를 한번 더 입력해주세요."));
			ToolTip->SetText(FText::FromString(*Temp));
			return;
		}
		else if (UserPW->GetText().ToString() != CheckPW->GetText().ToString())
		{
			Temp = FString::Printf(TEXT("두 비밀번호가 다릅니다."));
			ToolTip->SetText(FText::FromString(*Temp));
			return;
		}
		else if (UserNick->GetText().ToString().Len() < 1)
		{
			Temp = FString::Printf(TEXT("닉네임을 입력해주세요."));
			ToolTip->SetText(FText::FromString(*Temp));
			return;
		}
		Temp = FString::Printf(TEXT("계정 생성..."));
		ToolTip->SetText(FText::FromString(*Temp));
		UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (GI)
		{
			GI->HTTPRegistPost(FString(TEXT("http://127.0.0.1:3000/register")),

				NickName->GetText().ToString(),

				UserPW->GetText().ToString(),

				UserNick->GetText().ToString(),
				FHttpRequestCompleteDelegate::CreateUObject(this, &USignUpWidgetBase::SignUpResponseReceived));
		}
	}
}

void USignUpWidgetBase::SignUpResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		GLog->Log(FString::Printf(TEXT("Receive %s"), *Response->GetContentAsString()));

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString Result = JsonObject->GetStringField("result");
			if (Result.Compare(TEXT("false")) != 0)
			{
				//FString ID = JsonObject->GetStringField("id");
				//FString Password = JsonObject->GetStringField("password");
				BackToTitle();
			}
			else
			{
				ToolTip->SetText(FText::FromString(JsonObject->GetStringField("message")));
			}
		}
	}
}
