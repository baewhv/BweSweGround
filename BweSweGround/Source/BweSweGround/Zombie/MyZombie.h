// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyZombie.generated.h"

UENUM(BlueprintType)
enum class EZombieState :uint8
{
	None = 0	UMETA(DisplayName = "None"),
	Normal = 1	UMETA(DisplayName = "Normal"),
	Chase = 2	UMETA(DisplayName = "Chase"),
	Battle = 3	UMETA(DisplayName = "Battle"),
	Dead = 4	UMETA(DisplayName = "Dead"),

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
		float WalkSpeed = 100.0f;

	float RunSpeed = 600.0f;

	void Sprint_Start();
	void Sprint_End();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	uint8 bIsSprint : 1;

	uint8 bIsAttack : 1;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float CurrentHP = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UAnimMontage* DeadAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UAnimMontage* HitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EZombieState CurrentState = EZombieState::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBehaviorTree* ZombieAI;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPawnSensingComponent* PawnSensing;

	UFUNCTION()			//다른 컴포넌트의 델리게이트 호출방법(in h)
	void OnSeenPawn(APawn* Pawn);
	//void OnHearedNoise(APawn* Pawn, const FVector& Location, float Volume);

	void Attack();
};
