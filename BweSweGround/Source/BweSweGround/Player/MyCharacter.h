// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UENUM(BlueprintType)
enum class EInteraction :uint8
{
	None = 0	UMETA(DisplayName = "None"),
	Enemy = 1	UMETA(DisplayName = "Enemy"),
	Stealth = 2	UMETA(DisplayName = "Stealth"),
	Object = 3	UMETA(DisplayName = "Object"),

};

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
	class UMyWeaponComponent* Weapon;
	UPROPERTY(VisibleAnywhere)
	class UPawnNoiseEmitterComponent* PawnNoiseEmitter;

	EInteraction InteractionType = EInteraction::None;

	void TraceObject();

	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);

	void Sprint_Start();
	void Sprint_End();

	void Aim_Start();
	void Aim_End();

	UFUNCTION(Server, Reliable)
	void C2S_Aim_Start();
	void C2S_Aim_Start_Implementation();

	UFUNCTION(Server, Reliable)
	void C2S_Aim_End();
	void C2S_Aim_End_Implementation();

	void Crouch_Start();
	void Crouch_End();

	void StartFire();
	void StopFire();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetDie();
	void S2A_SetDie_Implementation();

	void Reload();
	void Reload_End();

	void Fire();
	void Stuck();


	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_Shot(FVector TraceStart, FVector TraceEnd);
	bool C2S_Shot_Validate(FVector TraceStart, FVector TraceEnd);
	void C2S_Shot_Implementation(FVector TraceStart, FVector TraceEnd);

	//Host만 사용 가능, 전체 클라이언트에 전송(Host포함)
	UFUNCTION(NetMulticast, Reliable)
	void S2A_SpawnDecalAndEffect(FHitResult OutHit, UParticleSystem* Hit, UMaterialInterface* DecalP);
	void S2A_SpawnDecalAndEffect_Implementation(FHitResult OutHit, UParticleSystem* Hit, UMaterialInterface* DecalP);

	void Interacted();
	void StartStealthKill();
	void EndStealthKill();

	FVector CameraLocation;
	FRotator CameraRotation;
	int32 SizeX;
	int32 SizeY;
	FVector WorldLocation;
	FVector WorldDirection;
	FVector TraceStart;
	FVector TraceEnd;

	AActor* InteractTarget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
	float WalkSpeed = 600.0f;

	float SprintSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
	float SprintSpeedValue = 1.5f;
	//
	//float CrouchSpeed = 1.0f;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
	//float CrouchSpeedValue = 1.0f;

	float AimSpeed = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
	float AimSpeedValue = 0.6f;

	float ForwardValue = 0.0f;
	float RightValue = 0.0f;
	
	UPROPERTY(VisibleAnywhere, Replicated)
	uint8 bIsSprint : 1;
	UPROPERTY(VisibleAnywhere, Replicated)
	uint8 bIsAim : 1;
	UPROPERTY(VisibleAnywhere, Replicated)
	uint8 bIsMotion : 1;
	UPROPERTY(VisibleAnywhere, Replicated)
	uint8 bIsAlive : 1;
	UPROPERTY(VisibleAnywhere, Replicated)
	uint8 bIsFire : 1;
	UPROPERTY(VisibleAnywhere, Replicated)
	uint8 bIsReloading : 1;
	UPROPERTY(VisibleAnywhere, Replicated)
	uint8 bIsStealthKill : 1;

	FRotator GetAimOffset() const;
	//숙이는 것은 기본 기능으로 있음. (Nav CanCrouch

	FTimerHandle FireTimer;


	UFUNCTION()
	void FireTimerFunction();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* BloodEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* HitEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class USoundBase* StuckSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UMaterialInterface* BulletDecal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	class UMaterialInterface* BulletDecalBlood;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;
	
	void SetDamage(float damage);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector NormalSpringArmPosition;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector CrouchSpringArmPosition;

	UFUNCTION(BlueprintCallable)
	FVector GetSpringArmPosition() const;
	
	UFUNCTION(BlueprintCallable)
	void SetSpringArmPosition(FVector NewPosition);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UCameraShake> FireCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	float CurrentHP = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimMontage* DeadAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimMontage* HitAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimMontage* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int CurrentBullet = 30;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxBullet = 30;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;
};
