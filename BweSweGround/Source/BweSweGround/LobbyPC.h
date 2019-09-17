// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPC.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API ALobbyPC : public APlayerController
{
	GENERATED_BODY()
public:
	int a;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class ULobbyWidgetBase> LobbyWidgetClass;

	virtual void BeginPlay() override;


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class ULobbyWidgetBase* LobbyWidget;

	//client to server
	UFUNCTION(Server, Reliable, withValidation)	//클라이언트에서 서버에서 보내는 행위.
	void C2S_SendMessage(const FText& Message);
	bool C2S_SendMessage_Validate(const FText& Message);
	void C2S_SendMessage_Implementation(const FText& Message);

	//server to client
	UFUNCTION(Client, Reliable)
	void S2C_SendMessage(const FText& Message);
	void S2C_SendMessage_Implementation(const FText& Message);
	
};
