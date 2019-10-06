// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "IHttpRequest.h"
#include "IHttpResponse.h"

#include "TitleWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API UTitleWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UEditableTextBox* NickName;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UEditableTextBox* UserPW;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UButton* ConnectButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UButton* MakeServerButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UButton* SignUpButton;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UEditableTextBox* ServerIP;

	UFUNCTION(BlueprintCallable)
		void Connect();
	UFUNCTION(BlueprintCallable)
		void MakeServer();
	UFUNCTION(BlueprintCallable)
		void OpenSighUp();


	void SetUserID(FString UserNick);

	void ConnectResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void MakeResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

};

