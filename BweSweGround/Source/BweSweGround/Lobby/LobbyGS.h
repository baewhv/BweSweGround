// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGS.generated.h"

/**
 * 
 */
UCLASS()
class BWESWEGROUND_API ALobbyGS : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(Replicated)//네트워크에 복제! 서버에 있는 값을 클라이언트로
	int LeftTime = 60;
	UPROPERTY(Replicated,VisibleAnywhere)
	int LeftAlive = 0;

	ALobbyGS();

	virtual void Tick(float DeltaSeconds) override;

	void SetAliver();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
