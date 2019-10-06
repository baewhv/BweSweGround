// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Http.h"
#include "Json.h"
#include "Engine/Engine.h"

FString UMyGameInstance::GetUserID()
{


	return UserID.IsEmpty() ? FString::Printf(TEXT("Noname%d"), FMath::RandRange(0, 100)) : UserID;
}

//void UMyGameInstance::Init()
//{
//	Super::Init();
//
//	//GetEngine()->OnNetworkFailure().AddUObject(this, &UMyGameInstance::HandleNetworkFailure);
//}

void UMyGameInstance::HTTPPost(FString URL, FString ID, FString Password, FHttpRequestCompleteDelegate ProcessRequestComplete)
{
	UE_LOG(LogClass, Warning, TEXT("Enter HTTPPost"));
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
	UE_LOG(LogClass, Warning, TEXT("Exit HTTPPost. %s"), );
}



//void UMyGameInstance::HTTPGet()
//{
//	Http = &FHttpModule::Get();
//
//	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
//	Request->OnProcessRequestComplete().BindUObject(this, &UMyGameInstance::HTTPResponseReceived);
//
//	FString url = FString(TEXT("http://home.junios.net:8000/ue4/login.php?id=junios&password=1234"));
//	Request->SetURL(url);
//	Request->SetVerb("GET");
//	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
//	Request->SetHeader("Content-Type", TEXT("application/json"));
//	Request->ProcessRequest();
//}

void UMyGameInstance::HTTPResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	GLog->Log(FString::Printf(TEXT("Callback")));
	if (bWasSuccessful)
	{
		//JSON 파싱
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		GLog->Log(FString::Printf(TEXT("Receive %s"), *Response->GetContentAsString()));

		//if (FJsonSerializer::Deserialize(Reader, JsonObject))
		//{
		//	FString ID = JsonObject->GetStringField("id");
		//	int Password = (int)JsonObject->GetNumberField("password");

		//	GLog->Log(FString::Printf(TEXT("Parsing %s %d"), *ID, Password));
		//}
	}
}

//void UMyGameInstance::HandleNetworkFailure(UWorld * World, UNetDriver * Driver, ENetworkFailure::Type Type, const FString & Message)
//{
//	UE_LOG(LogClass, Warning, TEXT("Net Error %s"), *ENetworkFailure::ToString(Type));
//}

