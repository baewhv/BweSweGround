// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MyWeaponComponent.h"
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
#include "Components/CapsuleComponent.h"
#include "MyCameraShake.h"
#include "Animation/AnimInstance.h"
#include "Zombie/MyZombie.h"

// Sets default values
AMyCharacter::AMyCharacter()
{

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Weapon = CreateDefaultSubobject<UMyWeaponComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	SpringArm->TargetArmLength = 200.0f;
	SpringArm->SocketOffset = FVector(0, 100.0f, 70.0f);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300.0f;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->CrouchedHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	//앉았을 때 캡슐 콜라이더 수정
	bIsMotion = false;
	bIsAlive = true;
	bIsReloading = false;
	bIsStealthKill = false;
	Tags.Add(TEXT("Character"));
	Tags.Add(TEXT("Player"));
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	NormalSpringArmPosition = GetSpringArmPosition();//서 있을 때
	CrouchSpringArmPosition = NormalSpringArmPosition + FVector(0, 0, -44.0f);	//앉아 있을 때.
	CurrentHP = MaxHP;	//체력 초기화
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * SprintSpeed * AimSpeed;

	//FString Temp = FString::Printf(TEXT("Pos (%f, %f)"),ForwardValue, RightValue);
	//UKismetSystemLibrary::PrintString(GetWorld(), Temp);
	TraceObject();
	
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

	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &AMyCharacter::Reload);

	PlayerInputComponent->BindAction(TEXT("Interaction"), IE_Pressed, this, &AMyCharacter::Interacted);
}

void AMyCharacter::TraceObject()
{
	APlayerController* PC = GetController<APlayerController>();
	if (PC)
	{
		int32 RandX = FMath::RandRange(-3, 3);
		PC->GetPlayerViewPoint(CameraLocation, CameraRotation);
		PC->GetViewportSize(SizeX, SizeY);
		PC->DeprojectScreenPositionToWorld(SizeX / 2, SizeY / 2, WorldLocation, WorldDirection);
		TraceStart = WorldLocation;
		TraceEnd = TraceStart + (WorldDirection * 300.0f);	//2미터

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
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

			if (OutHit.GetActor()->ActorHasTag(TEXT("Enemy")))
			{
				InteractionType = EInteraction::Enemy;
				float Distance = FVector::Distance(GetActorLocation(), OutHit.GetActor()->GetActorLocation());
				AMyZombie* Enemy = Cast<AMyZombie>(OutHit.GetActor());
				float Dot = Enemy->GetMesh()->GetAnimInstance()->CalculateDirection(GetActorLocation() - Enemy->GetActorLocation(), Enemy->GetActorRotation());
				//UE_LOG(LogClass, Warning, TEXT("Dist %f / Dot %f"), Distance, UKismetMathLibrary::Abs(Dot));
				if (Distance < 200.0f && UKismetMathLibrary::Abs(Dot) > 90.0f)
				{
					InteractionType = EInteraction::Stealth;
					InteractTarget = OutHit.GetActor();
				}
			}
		}
		else
		{
			InteractionType = EInteraction::None;
			InteractTarget = nullptr;
		}
	}
}

void AMyCharacter::MoveForward(float Value)
{
	if (!bIsAlive || bIsStealthKill)
	{
		return;
	}
	if ((Value != 0 && (!bIsMotion )) || !bIsAlive )
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
	if (!bIsAlive || bIsStealthKill)
	{
		return;
	}
	if (Value != 0 && (!bIsMotion))
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
	if (bIsMotion || !bIsAlive || bIsStealthKill)
	{
		return;
	}
	if (bIsAim)
	{
		Aim_End();
	}
	if(bIsCrouched)
	{
		UnCrouch();
	}
	if (bIsReloading)
	{
		bIsReloading = false;
	}
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bIsSprint = true;
	SprintSpeed = SprintSpeedValue;
}

void AMyCharacter::Sprint_End()
{
	if (bIsSprint)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		bIsSprint = false;
		SprintSpeed = 1.0f;
	}
}

void AMyCharacter::Aim_Start()
{
	if (bIsSprint)
	{
		Sprint_End();
	}
	bIsAim = true;
	AimSpeed = AimSpeedValue;
	//SpringArm->TargetArmLength = 80.0f;	->카메라로(MyPlayerCameraManager) 조정
}

void AMyCharacter::Aim_End()
{
	bIsAim = false;
	AimSpeed = 1.0f;
	//SpringArm->TargetArmLength = 200.0f;
}

void AMyCharacter::Crouch_Start()
{
	if (bIsMotion)
	{
		return;
	}
	if (bIsSprint)
	{
		Sprint_End();
	}
	////SpringArm->SocketOffset = FVector(0, 100.0f, 70.0f);
	//bIsCrouch = true;
	//CrouchSpeed = 0.6f;
	if (CanCrouch())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void AMyCharacter::Crouch_End()
{
	//bIsCrouch = false;
	//CrouchSpeed = 1.0f;
}

void AMyCharacter::StartFire()
{
	if (bIsAlive)
	{
		bIsFire = true;
		if (CurrentBullet != 0)
		{
			Fire();
		}
		else
		{
			Stuck();
		}
	}
}

void AMyCharacter::StopFire()
{
	bIsFire = false;
}

void AMyCharacter::SetDie()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);		//콜리전 끄기
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = false;
	GetCharacterMovement()->NavAgentProps.bCanJump = false;
	bIsAlive = false;
}

void AMyCharacter::Reload()
{
	if ((CurrentBullet != MaxBullet) && bIsAlive && !bIsReloading)
	{
		if (bIsSprint)
		{
			Sprint_End();
		}
		bIsReloading = true;
		PlayAnimMontage(ReloadAnimation, 1.0f, FName(TEXT("Reload_Normal")));
	}
}

void AMyCharacter::Reload_End()
{
	bIsReloading = false;
	CurrentBullet = MaxBullet;
}

void AMyCharacter::Fire()
{
	if (!bIsFire || bIsReloading)
	{
		return;
	}
	if (bIsSprint)
	{
		Sprint_End();
	}
	if (CurrentBullet != 0)
	{
		CurrentBullet--;
		APlayerController* PC = GetController<APlayerController>();
		if (PC)
		{
			int32 RandX = FMath::RandRange(-3, 3);
			//PC->GetPlayerViewPoint(CameraLocation, CameraRotation);
			//PC->GetViewportSize(SizeX, SizeY);
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
				
				if (OutHit.GetActor()->ActorHasTag(TEXT("Character")))
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
		PlayerRotation.Pitch += FMath::RandRange(0.5f, 1.0f);
		GetController()->SetControlRotation(PlayerRotation);

		if (FireSound && MuzzleFlash)	//보호 차원
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, Weapon->GetSocketLocation(TEXT("Muzzle")));
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, Weapon->GetSocketTransform(TEXT("Muzzle")));
		}

		if (PC)
		{
			//PC->PlayerCameraManager->PlayCameraShake(UMyCameraShake::StaticClass());
			//PC->PlayerCameraManager->PlayWorldCameraShake();	//모든 곳 흔들기?
			PC->PlayerCameraManager->PlayCameraShake(FireCameraShake);	//카메라 쉐이크를 선택하게끔
		}

		if (bIsFire)
		{
			GetWorldTimerManager().SetTimer(FireTimer, this, &AMyCharacter::FireTimerFunction, 0.12);
		}
	}
	else
	{
		Stuck();
	}
}

void AMyCharacter::Stuck()
{
	if (!bIsFire)
	{
		return;
	}
	if (bIsSprint)
	{
		Sprint_End();
	}
	if (StuckSound)	//보호 차원
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, Weapon->GetSocketLocation(TEXT("Muzzle")));
	}
	bIsFire = false;
}

void AMyCharacter::Interacted()
{
	switch (InteractionType)
	{
	case EInteraction::Enemy:
		UE_LOG(LogClass, Warning, TEXT("Enemy"));
		break;
	case EInteraction::None:
		UE_LOG(LogClass, Warning, TEXT("None"));
		break;
	case EInteraction::Stealth:
		UE_LOG(LogClass, Warning, TEXT("Stealth"));
		StartStealthKill();
		break;
	}
	
}

void AMyCharacter::StartStealthKill()
{
	if (!bIsStealthKill)
	{
		bIsCrouched = false;
		bIsSprint = false;
		bIsAim = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		AMyZombie* Enemy = Cast<AMyZombie>(InteractTarget);
		FVector NewLocation = FVector(GetActorLocation() + (GetActorForwardVector() * 50.0f) + (GetActorRightVector() * -20.0f));
		bIsStealthKill = true;
		Enemy->SetActorRotation(GetActorRotation());
		Enemy->SetActorLocation(NewLocation);
		Enemy->bIsStealthKilled = true;
		Enemy->SetDie();
	}
}

void AMyCharacter::EndStealthKill()
{
	UE_LOG(LogClass, Warning, TEXT("EndStelthKill! %d"), bIsStealthKill);
	if (bIsStealthKill)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		bIsStealthKill = false;
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
	if (CurrentHP == 0)	//또 죽이지 않도록 방지
	{
		return 0;
	}
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
			CurrentHP = 0;
			//SetDamage(DamageAmount*10);
		}
		else
		{
			CurrentHP -= DamageAmount;
			//SetDamage(DamageAmount);
		}
	}
	else
	{
		CurrentHP -= DamageAmount;
		//SetDamage(DamageAmount);
	}

	FString HitName = FString::Printf(TEXT("Hit_%d"), FMath::RandRange(1, 4));
	PlayAnimMontage(HitAnimation, 1.0f, FName(*HitName));
	bIsReloading = false;
	CurrentHP = FMath::Clamp<float>(CurrentHP, 0, MaxHP);	//체력을 보정(-로 떨어져도 0으로 고정)

	if (CurrentHP == 0)
	{
		bIsReloading = false;
		SetDie();
		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);		//콜리전 끄기
		//GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);	//메쉬 물리 체크 켜기.
		//GetMesh()->SetSimulatePhysics(true);
		//bIsAlive = false;

		FString DeadName = FString::Printf(TEXT("Death_%d"), FMath::RandRange(1, 3));
		PlayAnimMontage(DeadAnimation, 1.0f, FName(*DeadName));
		//SetLifeSpan(5.0f);
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AMyCharacter::SetDamage(float damage)
{
	if (CurrentHP <= damage)
	{
		CurrentHP = 0.0f;
		if (bIsAlive)
		{
			bIsAlive = false;
			//GetWorld()->SpawnActor<AMyCharacter>(AMyCharacter::StaticClass(),);
			UE_LOG(LogClass, Warning, TEXT("Die"));
		}
	}
	else
	{
		CurrentHP -= damage;
		UE_LOG(LogClass, Warning, TEXT("Get %f Damage! %f Left"), damage, CurrentHP);

	}
}

FVector AMyCharacter::GetSpringArmPosition() const
{
	return SpringArm->GetRelativeTransform().GetLocation();
}

void AMyCharacter::SetSpringArmPosition(FVector NewPosition)
{
	SpringArm->SetRelativeLocation(NewPosition);
}
