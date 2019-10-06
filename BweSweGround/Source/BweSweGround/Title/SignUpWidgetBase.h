// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "IHttpRequest.h"
#include "IHttpResponse.h"

#include "SignUpWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API USignUpWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UEditableTextBox*	NickName;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UEditableTextBox*	UserPW;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UEditableTextBox*	CheckPW;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UEditableTextBox* UserNick;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UButton* SignUpButton;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UButton* BackButton;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UTextBlock* ToolTip; 

	UFUNCTION(BlueprintCallable)
		void BackToTitle();
	UFUNCTION(BlueprintCallable)
		void SignUp();

	void SignUpResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
};
