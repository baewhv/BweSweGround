// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Http.h"
#include "Json.h"
#include "Engine/Engine.h"

FString UMyGameInstance::GetUserID()
{
	if (NickName.IsEmpty())
	{
		NickName = FString::Printf(TEXT("Noname%d"), FMath::RandRange(0, 100));
	}
	return NickName;
}

void UMyGameInstance::HTTPPost(FString URL, FString ID, FString Password, FHttpRequestCompleteDelegate ProcessRequestComplete)
{
	Http = &FHttpModule::Get();

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	//콜백 등록
	Request->OnProcessRequestComplete() = ProcessRequestComplete;

	//FString url = FString(TEXT("http://127.0.0.1:3000/login"));
	FString PostParameters = FString::Printf(TEXT("userid=%s"), *ID)
		+ FString::Printf(TEXT("&password=%s"), *Password);
	Request->SetURL(URL);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));	//주소의 문자를 인코딩
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();
}

void UMyGameInstance::HTTPRegistPost(FString URL, FString ID, FString Password, FString UserNick, FHttpRequestCompleteDelegate ProcessRequestComplete)
{
	Http = &FHttpModule::Get();

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	//콜백 등록
	Request->OnProcessRequestComplete() = ProcessRequestComplete;

	FString PostParameters = FString::Printf(TEXT("userid=%s"), *ID)
		+ FString::Printf(TEXT("&nickname=%s"), *UserNick)
		+ FString::Printf(TEXT("&password=%s"), *Password);
	Request->SetURL(URL);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));	//주소의 문자를 인코딩
	Request->SetContentAsString(PostParameters);
	Request->ProcessRequest();
}




void UMyGameInstance::HTTPResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	GLog->Log(FString::Printf(TEXT("Callback")));
	if (bWasSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		GLog->Log(FString::Printf(TEXT("Receive %s"), *Response->GetContentAsString()));

	}
}


