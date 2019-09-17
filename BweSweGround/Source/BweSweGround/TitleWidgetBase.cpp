// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidgetBase.h"
#include "Components/EditableTextBox.h"	//기존 UI의 텍스트 박스
#include "Components/Button.h"	//기존 UI의 버튼
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"

void UTitleWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	UserID = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("UserID")));
	UserPW = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("Password")));
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
		SetUserID();
		FName Stage = FName(*ServerIP->GetText().ToString());
		UE_LOG(LogClass, Warning, TEXT("ServerIP : %s"), *ServerIP->GetText().ToString());
		UGameplayStatics::OpenLevel(GetWorld(), Stage);
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("ServerIP"));
	}
	
}

void UTitleWidgetBase::MakeServer()
{
	SetUserID();
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Lobby")), true, TEXT("listen"));
}

void UTitleWidgetBase::SetUserID()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		GI->UserID = UserID->GetText().ToString();
	}
}