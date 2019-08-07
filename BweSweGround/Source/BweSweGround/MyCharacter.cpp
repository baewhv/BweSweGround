// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h" 


// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	SpringArm->TargetArmLength = 200.0f;
	SpringArm->SocketOffset = FVector(0, 100.0f, 70.0f);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300.0f;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	bIsMotion = false;
	bIsAlive = true;
	HP = 100.0f;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * SprintSpeed * CouchSpeed * AimSpeed;

	//FString Temp = FString::Printf(TEXT("Pos (%f, %f)"),ForwardValue, RightValue);
	//UKismetSystemLibrary::PrintString(GetWorld(), Temp);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AMyCharacter::Sprint_Start);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AMyCharacter::Sprint_End);
	
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Pressed, this, &AMyCharacter::Aim_Start);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Released, this, &AMyCharacter::Aim_End);

	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &AMyCharacter::Crouch_Start);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &AMyCharacter::Crouch_End);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMyCharacter::Jump);	//캐릭터.cpp에서 상속받은 함수를 불러옴
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AMyCharacter::StopJumping);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMyCharacter::StartFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AMyCharacter::StopFire);
}

void AMyCharacter::MoveForward(float Value)
{
	if (Value != 0 && !bIsMotion)
	{
		ForwardValue = Value;
		AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	}
	else
	{
		ForwardValue = 0.0f;
	}
}

void AMyCharacter::MoveRight(float Value)
{
	if (Value != 0 && !bIsMotion)
	{
		RightValue = Value;
		AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	}
	else
	{
		RightValue = 0.0f;
	}
}

void AMyCharacter::LookUp(float Value)
{
	if (Value != 0)
	{
		AddControllerPitchInput(Value);
	}
}

void AMyCharacter::Turn(float Value)
{
	if (Value != 0)
	{
		AddControllerYawInput(Value);
	}
}

void AMyCharacter::Sprint_Start()
{
	if (bIsAim)
	{
		Aim_End();
		Crouch_End();
	}
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bIsSprint = true;
	SprintSpeed = 1.2f;
}

void AMyCharacter::Sprint_End()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bIsSprint = false;
	SprintSpeed = 1.0f;
}

void AMyCharacter::Aim_Start()
{
	if (bIsSprint)
	{
		Sprint_End();
	}
	bIsAim = true;
	AimSpeed = 0.6f;
	SpringArm->TargetArmLength = 80.0f;
}

void AMyCharacter::Aim_End()
{
	bIsAim = false;
	AimSpeed = 1.0f;
	SpringArm->TargetArmLength = 200.0f;
}

void AMyCharacter::Crouch_Start()
{
	if (bIsSprint)
	{
		Sprint_End();
	}
	//SpringArm->SocketOffset = FVector(0, 100.0f, 70.0f);
	bIsCrouch = true;
	CouchSpeed = 0.6f;
}

void AMyCharacter::Crouch_End()
{
	bIsCrouch = false;
	CouchSpeed = 1.0f;
}

void AMyCharacter::StartFire()
{
	bIsFire = true;
	Fire();
}

void AMyCharacter::StopFire()
{
	bIsFire = false;
}

void AMyCharacter::Fire()
{
	if (!bIsFire)
	{
		return;
	}

	FVector CameraLocation;
	FRotator CameraRotation;
	int32 SizeX;
	int32 SizeY;
	FVector WorldLocation;
	FVector WorldDirection;
	FVector TraceStart;
	FVector TraceEnd;
	APlayerController* PC = GetController<APlayerController>();
	if (PC)
	{
		int32 RandX = FMath::RandRange(-3, 3);
		PC->GetPlayerViewPoint(CameraLocation, CameraRotation);
		PC->GetViewportSize(SizeX, SizeY);
		PC->DeprojectScreenPositionToWorld(SizeX / 2 - RandX, SizeY / 2, WorldLocation, WorldDirection);
		TraceStart = WorldLocation;
		TraceEnd = TraceStart + (WorldDirection * 90000.0f);	//90미터


		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
		ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

		TArray<AActor*>IgnoreActors;
		IgnoreActors.Add(this);

		FHitResult OutHit;

		bool bResult = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			TraceStart,
			TraceEnd,
			ObjectType,
			false,
			IgnoreActors,
			EDrawDebugTrace::None,	//그리지 않겠다. / ForDuration -> 일정 시간만
			OutHit,
			true,
			FLinearColor::Red, FLinearColor::Green, 5.0f);
		if (bResult)
		{
			//UE_LOG(LogClass, Warning, TEXT("Hit %s"), *OutHit.GetActor()->GetName());
			UParticleSystem* HitP;
			UMaterialInterface* DecalP;

			if (OutHit.GetActor()->ActorHasTag(TEXT("Player")))
			{
				HitP = BloodEffect;
				DecalP = BulletDecalBlood;
				//UGameplayStatics::ApplyDamage(OutHit.GetActor(), 10.0f, nullptr, this, nullptr);

			}
			else
			{
				HitP = HitEffect;
				DecalP = BulletDecal;
			}
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitP, OutHit.ImpactPoint + OutHit.ImpactNormal * 5.0f, OutHit.ImpactNormal.Rotation());
			UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalP, FVector(5, 5, 5), OutHit.ImpactPoint, OutHit.ImpactNormal.Rotation());
			Decal->SetFadeScreenSize(0.001f);

			//범위 대미지
			//UGameplayStatics::ApplyRadialDamage(GetWorld(), 30.0f, OutHit.ImpactPoint, 500.0f, nullptr, IgnoreActors, this, GetController(), false, ECollisionChannel::ECC_Visibility);

			//포인트 대미지
			UGameplayStatics::ApplyPointDamage(OutHit.GetActor(), 10.0f, TraceEnd - TraceStart, OutHit, GetController(), this, nullptr);

			//if (OutHit.GetActor()->ActorHasTag(TEXT("Player")))
			//{
			//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodEffect, OutHit.ImpactPoint + OutHit.ImpactNormal * 5.0f, OutHit.ImpactNormal.Rotation());
			//	UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletDecalBlood, FVector(5, 5, 5), OutHit.ImpactPoint, OutHit.ImpactNormal.Rotation());
			//	Decal->SetFadeScreenSize(0.001f);
			//	UGameplayStatics::ApplyDamage(OutHit.GetActor(), 10.0f, nullptr, this, nullptr);
			//}
			//else
			//{
			//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, OutHit.ImpactPoint + OutHit.ImpactNormal * 5.0f, OutHit.ImpactNormal.Rotation());
			//	UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletDecal, FVector(5, 5, 5), OutHit.ImpactPoint, OutHit.ImpactNormal.Rotation());
			//	Decal->SetFadeScreenSize(0.001f);
			//}
		}
	}
	FRotator PlayerRotation = GetControlRotation();
	PlayerRotation.Pitch += FMath::RandRange(0.5f,1.0f);
	GetController()->SetControlRotation(PlayerRotation);

	if (FireSound && MuzzleFlash)	//보호 차원
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, Weapon->GetSocketLocation(TEXT("Muzzle")));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, Weapon->GetSocketTransform(TEXT("Muzzle")));
	}

	if (bIsFire)
	{
		GetWorldTimerManager().SetTimer(FireTimer, this, &AMyCharacter::FireTimerFunction, 0.12);
	}
}


FRotator AMyCharacter::GetAimOffset() const
{
	//FVector AimDirWS = GetBaseAimRotation().Vector();
	//FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);	//월드 방향을 로컬방향으로 전환.
	//FRotator AimRotLS = AimDirLS.Rotation();

	return ActorToWorld().InverseTransformVectorNoScale(GetBaseAimRotation().Vector()).Rotation();
}

void AMyCharacter::FireTimerFunction()
{
	Fire();
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* RadialDamageEvent = (FRadialDamageEvent*)(&DamageEvent);
		if (RadialDamageEvent)
		{
			SetDamage(DamageAmount);
		}
		
	}
	else if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);
		if (PointDamageEvent->HitInfo.BoneName.Compare(TEXT("Head")) == 0)
		{
			SetDamage(DamageAmount*100);
		}
		else
		{
			SetDamage(DamageAmount);
		}
	}
	else
	{
		SetDamage(DamageAmount);
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AMyCharacter::SetDamage(float damage)
{

	if (HP <= damage)
	{
		HP = 0.0f;
		if (bIsAlive)
		{
			bIsAlive = false;
			//GetWorld()->SpawnActor<AMyCharacter>(AMyCharacter::StaticClass(),);
			UE_LOG(LogClass, Warning, TEXT("Die"));
		}
	}
	else
	{
		HP -= damage;
		UE_LOG(LogClass, Warning, TEXT("Get %f Damage! %f Left"), damage, HP);

	}
}
