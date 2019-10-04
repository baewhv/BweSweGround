// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpawnManager.h"
#include "MyZombie.generated.h"

UENUM(BlueprintType)
enum class EZombieState :uint8
{
	None = 0	UMETA(DisplayName = "None"),
	Normal = 1	UMETA(DisplayName = "Normal"),
	Chase = 2	UMETA(DisplayName = "Chase"),
	Battle = 3	UMETA(DisplayName = "Battle"),
	Dead = 4	UMETA(DisplayName = "Dead"),
	Alert = 5	UMETA(DisplayName = "Alert"),

};

UCLASS()
class BWESWEGROUND_API AMyZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Server, Reliable)
		void C2S_initProperty();
		void C2S_initProperty_Implementation();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UPawnSensingComponent* PawnSensing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UWidgetComponent* HPWidget;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
		float WalkSpeed = 100.0f;
		float RunSpeed = 600.0f;
	
		void Sprint_Start();
		void Sprint_End();

	UFUNCTION(NetMulticast, Reliable)
		void C2S_Sprint_Start();
		void C2S_Sprint_Start_Implementation();

	UFUNCTION(NetMulticast, Reliable)
		void C2S_Sprint_End();
		void C2S_Sprint_End_Implementation();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", Replicated)
		uint8 bIsSprint : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", Replicated)
		uint8 bIsAttack : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		uint8 bIsStealthKilled : 1;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;
	
	UFUNCTION(NetMulticast, Reliable)
		void S2A_SetDie();
		void S2A_SetDie_Implementation();
	UFUNCTION()
		void SetHPWidget_OnRep();
	UFUNCTION()			//다른 컴포넌트의 델리게이트 호출방법(in h)
		void OnSeenPawn(APawn* PP);
	UFUNCTION()
		void OnHearedNoise(APawn* pawn, const FVector& Location, float Volume);
	
	void Attack();
	
	void getSpawnPoint(ASpawnManager* point);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = SetHPWidget_OnRep)
		float CurrentHP = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UAnimMontage* DeadAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UAnimMontage* HitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
		EZombieState CurrentState = EZombieState::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UBehaviorTree* ZombieAI;
	UPROPERTY(VisibleAnywhere)
		ASpawnManager* SpawnPoint;


};
