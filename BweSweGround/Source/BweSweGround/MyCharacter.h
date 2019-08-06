// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"
#define DEFAULT 1.0f;

UCLASS()
class BWESWEGROUND_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Weapon;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);

	void Sprint_Start();
	void Sprint_End();

	void Aim_Start();
	void Aim_End();

	void Crouch_Start();
	void Crouch_End();



	float WalkSpeed = 600.0f;
	float SprintSpeed = 1.0f;
	float CouchSpeed = 1.0f;
	float AimSpeed = 1.0f;

	float ForwardValue = 0.0f;
	float RightValue = 0.0f;
	
	uint8 bIsSprint : 1;
	uint8 bIsAim : 1;
	uint8 bIsCrouch : 1;
	uint8 bIsMotion : 1;

	FRotator GetAimOffset() const;
	//숙이는 것은 기본 기능으로 있음. (Nav CanCrouch



};
