// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePC.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API AGamePC : public APlayerController
{
	GENERATED_BODY()

public:
	AGamePC();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UGameWidgetBase* GameWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameWidgetBase> GameWidgetClass;

	void ShowGameWidget();
	void HideGameWidget();

	void ShowItemName(FString Name);
	void HideItemName();

	void SetHPBar(float hp);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInventoryWidgetBase* InventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UInventoryWidgetBase> InventoryWidgetClass;

	void ShowInventory();
	void HideInventory();

	virtual void SetupInputComponent() override;

	void InventoryToggle();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UResultWidgetBase* ResultWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UResultWidgetBase> ResultWidgetClass;

	void ShowResult(uint8 result);
	
	void ExitGame();

	//UPROPERTY(VisibleAnywhere, Replicated)
	uint8 bIsCurrentPlayerDie : 1;

	UFUNCTION(Server, Reliable)	//클라이언트에서 서버에서 보내는 행위.
	void C2S_SendMessage(const FText& Message);
	void C2S_SendMessage_Implementation(const FText& Message);

	UFUNCTION(Client, Reliable)
	void S2C_SendLog(const FText& Message);
	void S2C_SendLog_Implementation(const FText& Message);
};
