// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidgetBase.h"
#include "Components/EditableTextBox.h"	//기존 UI의 텍스트 박스
#include "Components/Button.h"	//기존 UI의 버튼
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "Http.h"
#include "Json.h"

void UTitleWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	UserID = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("UserID")));
	UserPW = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("UserPW")));
	ServerIP = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ServerIP")));

	ConnectButton = Cast<UButton>(GetWidgetFromName(TEXT("ConnectButton")));
	if (ConnectButton)
	{
		ConnectButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::Connect);
	}
	MakeServerButton = Cast<UButton>(GetWidgetFromName(TEXT("MakeServerButton")));
	if (MakeServerButton)
	{
		MakeServerButton->OnClicked.AddDynamic(this, &UTitleWidgetBase::MakeServer);
	}
}

void UTitleWidgetBase::Connect()
{
	if (ServerIP)
	{
		
		UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (GI && UserID && UserPW)
		{
			GI->HTTPPost(FString(TEXT("http://127.0.0.1:3000/login")),
				UserID->GetText().ToString(),
				UserPW->GetText().ToString(),
				FHttpRequestCompleteDelegate::CreateUObject(this, &UTitleWidgetBase::ConnectResponseReceived));
			
		}
		else if(!GI)
		{
			UE_LOG(LogClass, Warning, TEXT("connect error(gi)"));
		}
		else if (!UserID)
		{
			UE_LOG(LogClass, Warning, TEXT("connect error(userid)"));
		}
		else if (!UserPW)
		{
			UE_LOG(LogClass, Warning, TEXT("connect error(pw)"));
		}
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("ServerIP"));
	}
	
}

void UTitleWidgetBase::MakeServer()
{
	
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI && UserID && UserPW)
	{
		GI->HTTPPost(FString(TEXT("http://127.0.0.1:3000/login")),
			UserID->GetText().ToString(),
			UserPW->GetText().ToString(),
			FHttpRequestCompleteDelegate::CreateUObject(this, &UTitleWidgetBase::MakeResponseReceived));
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("make server error"));
	}
	
}

void UTitleWidgetBase::SetUserID()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		GI->UserID = UserID->GetText().ToString();
	}
}

void UTitleWidgetBase::ConnectResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
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
				FString ID = JsonObject->GetStringField("id");
				FString Password = JsonObject->GetStringField("password");
				SetUserID();
				UGameplayStatics::OpenLevel(GetWorld(), FName(*ServerIP->GetText().ToString()));

			}
			else
			{
				UE_LOG(LogClass, Warning, TEXT("false!"));
			}
		}
	}
}

void UTitleWidgetBase::MakeResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
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
				FString ID = JsonObject->GetStringField("id");
				FString Password = JsonObject->GetStringField("password");
				SetUserID();
				UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Lobby")), true, TEXT("listen"));

			}
			else
			{
				UE_LOG(LogClass, Warning, TEXT("false!"));
			}
		}
	}
}

