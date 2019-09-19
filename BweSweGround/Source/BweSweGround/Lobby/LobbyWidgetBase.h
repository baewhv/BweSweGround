// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API ULobbyWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UTextBlock* AliveCount;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UTextBlock* PublicMessageBox;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UScrollBox* ChattingBox;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UEditableTextBox* ChatBox;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UButton* StartGameButton;

	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void OnTextCommit(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION(BlueprintCallable)
	void AddMessage(const FText& Text);

	UFUNCTION(BlueprintCallable)
	void SetLeftTime(int32 LeftTime);

	UFUNCTION(BlueprintCallable)
	void SetLeftAlive(int32 LeftAlive);

	UFUNCTION(BlueprintNativeEvent)
	void PlayStartButton();
	void PlayStartButton_Implementation();
};
