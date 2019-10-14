// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "MyWeaponComponent.h"
#include "Item/MasterItem.h"
#include "MyCameraShake.h"
#include "Animation/AnimInstance.h"
#include "Zombie/MyZombie.h"
#include "Game/GamePC.h"
#include "Game/GameGM.h"
#include "Game/GameGS.h"
#include "Game/GamePS.h"
#include "Game/GameWidgetBase.h"
#include "Item/InventoryWidgetBase.h"

#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/DecalComponent.h" 
#include "Components/CapsuleComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "UnrealNetwork.h"
#include "MyGameInstance.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsGameDone = false;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Weapon = CreateDefaultSubobject<UMyWeaponComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));

	PawnNoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitter"));

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	SpringArm->TargetArmLength = 200.0f;
	SpringArm->SocketOffset = FVector(0, 50.0f, 70.0f);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300.0f;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->CrouchedHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	//앉았을 때 캡슐 콜라이더 수정
	
	Tags.Add(TEXT("Character"));
	Tags.Add(TEXT("Player"));
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	NormalSpringArmPosition = GetSpringArmPosition();//서 있을 때
	CrouchSpringArmPosition = NormalSpringArmPosition + FVector(0, 0, -44.0f);	//앉아 있을 때.
	SetCurrentBulletUI();
	SetCurrentHPUI_OnRep();
	SetCurrentDegreeUI();

	GetWorldTimerManager().SetTimer(LookItemHandler, this, &AMyCharacter::TraceObject, 0.1f, true);;
	InitProperty();

	UMyGameInstance* GI = GetGameInstance<UMyGameInstance>();
	if (GI)
	{
		NickName = GI->NickName;
		C2S_SetName(NickName);
	}
}


// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * SprintSpeed * AimSpeed;

	//FString Temp = FString::Printf(TEXT("Pos (%f, %f)"),ForwardValue, RightValue);
	//UKismetSystemLibrary::PrintString(GetWorld(), Temp);
	
}


void AMyCharacter::InitProperty()
{
	bIsMotion = false;
	bIsAlive = true;
	bIsReloading = false;
	bIsStealthKill = false;
	CurrentHP = MaxHP;	//체력 초기화
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

	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &AMyCharacter::Crouch_Toggle);
	//PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &AMyCharacter::Crouch_End);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMyCharacter::Jump);	//UCharacter.h에서 상속받은 함수를 불러옴
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AMyCharacter::StopJumping);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMyCharacter::StartFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AMyCharacter::StopFire);

	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &AMyCharacter::Reload);

	PlayerInputComponent->BindAction(TEXT("Interaction"), IE_Pressed, this, &AMyCharacter::Interacted);
}

void AMyCharacter::TraceObject()
{

	AGamePC* PC = GetController<AGamePC>();
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
		ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

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
			//UE_LOG(LogClass, Warning, TEXT("bResult Hit %s"), *OutHit.GetActor()->GetName());

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
			if (OutHit.GetActor()->ActorHasTag(TEXT("Item")))
			{
				AMasterItem* Item = Cast<AMasterItem>(OutHit.Actor);
				if (Item)
				{
					InteractionType = EInteraction::Object;
					InteractTarget = OutHit.GetActor();
					PC->ShowItemName(Item->ItemData.ItemName);
					PickUpItem = OutHit;
				}
				//UE_LOG(LogClass, Warning, TEXT("%s"), *OutHit.Actor->GetName())
			}
			else
			{
				PC->HideItemName();
				PickUpItem = FHitResult();
			}
		}
		else
		{
			InteractionType = EInteraction::None;
			InteractTarget = nullptr;
			PC->HideItemName();
			PickUpItem = FHitResult();
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
		SetCurrentDegreeUI();
	}
}

void AMyCharacter::Sprint_Start()
{
	if (bIsMotion || !bIsAlive || bIsStealthKill || (ForwardValue == 0.0f && RightValue == 0.0f))
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
	C2S_Sprint_Start();
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
	C2S_Sprint_End();
}

void AMyCharacter::C2S_SetName_Implementation(FString name)
{
	NickName = name;
}

void AMyCharacter::C2S_Sprint_Start_Implementation()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bIsSprint = true;
	SprintSpeed = SprintSpeedValue;
}

void AMyCharacter::C2S_Sprint_End_Implementation()
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
	C2S_Aim_Start();
	//SpringArm->TargetArmLength = 80.0f;	->카메라로(MyPlayerCameraManager) 조정
}

void AMyCharacter::Aim_End()
{
	bIsAim = false;
	AimSpeed = 1.0f;
	//SpringArm->TargetArmLength = 200.0f;
	C2S_Aim_End();
}

void AMyCharacter::C2S_Aim_Start_Implementation()
{
	if (bIsSprint)
	{
		Sprint_End();
	}
	bIsAim = true;
	AimSpeed = AimSpeedValue;
}

void AMyCharacter::C2S_Aim_End_Implementation()
{
	bIsAim = false;
	AimSpeed = 1.0f;
}

void AMyCharacter::Crouch_Toggle()	//현재 토글로써 기능.
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
	C2S_StartFire();
}

void AMyCharacter::C2S_StartFire_Implementation()
{
	if (bIsAlive)
	{
		bIsFire = true;
	}
}

void AMyCharacter::StopFire()
{
	bIsFire = false;
	C2S_StopFire();
}

void AMyCharacter::C2S_StopFire_Implementation()
{
	bIsFire = false;
}

void AMyCharacter::S2A_SetDie_Implementation()
{
	bIsReloading = false;
	FString DeadName = FString::Printf(TEXT("Death_%d"), FMath::RandRange(1, 3));
	PlayAnimMontage(DeadAnimation, 1.0f, FName(*DeadName));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);		//콜리전 끄기
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		DisableInput(PC);
	}
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = false;
	GetCharacterMovement()->NavAgentProps.bCanJump = false;
	bIsAlive = false;
}

void AMyCharacter::Reload()
{
	if ((CurrentBullet != MaxBullet) && (Magazine != 0) && bIsAlive && !bIsReloading)
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

	if (Magazine >= MaxBullet)
	{
		Magazine = Magazine - (MaxBullet - CurrentBullet);
		CurrentBullet += MaxBullet - CurrentBullet;
	}
	else
	{
		if (Magazine + CurrentBullet > MaxBullet)
		{
			Magazine = (Magazine + CurrentBullet) - MaxBullet;
			CurrentBullet = MaxBullet;
		}
		else
		{
			int Mag = Magazine;
			Magazine = 0;
			CurrentBullet = Mag + CurrentBullet;
		}
	}
	SetCurrentBulletUI();
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
		PawnNoiseEmitter->MakeNoise(this, 1.0f, GetActorLocation());
		AGamePC* PC = GetController<AGamePC>();
		if (PC)
		{
			CurrentBullet--;
			SetCurrentBulletUI();
			int32 RandX = FMath::RandRange(-3, 3);
			PC->DeprojectScreenPositionToWorld(SizeX / 2 - RandX, SizeY / 2, WorldLocation, WorldDirection);
			TraceStart = WorldLocation;
			TraceEnd = TraceStart + (WorldDirection * 90000.0f);	//90미터
			C2S_Shot(TraceStart, TraceEnd);
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



bool AMyCharacter::C2S_Shot_Validate(FVector _TraceStart, FVector _TraceEnd)
{
	return true;
}

void AMyCharacter::C2S_Shot_Implementation(FVector _TraceStart, FVector _TraceEnd)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	TArray<AActor*>IgnoreActors;
	IgnoreActors.Add(this);

	FHitResult OutHit;

	bool bResult = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		_TraceStart,
		_TraceEnd,
		ObjectType,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,	//그리지 않겠다. / ForDuration -> 일정 시간만
		OutHit,
		true,
		FLinearColor::Red, FLinearColor::Green, 5.0f);
	if (bResult)
	{
		UParticleSystem* HitP;
		UMaterialInterface* DecalP;
		if (OutHit.GetActor()->ActorHasTag(TEXT("Character")))
		{
			HitP = BloodEffect;
			DecalP = BulletDecalBlood;
		}
		else
		{
			PawnNoiseEmitter->MakeNoise(OutHit.GetActor(), 1.0f, OutHit.Location);
			HitP = HitEffect;
			DecalP = BulletDecal;
		}

		S2A_SpawnDecalAndEffect(OutHit, HitP, DecalP);
		//범위 대미지
		//UGameplayStatics::ApplyRadialDamage(GetWorld(), 30.0f, OutHit.ImpactPoint, 500.0f, nullptr, IgnoreActors, this, GetController(), false, ECollisionChannel::ECC_Visibility);
		//포인트 대미지
		UGameplayStatics::ApplyPointDamage(OutHit.GetActor(), 10.0f, _TraceEnd - _TraceStart, OutHit, GetController(), this, nullptr);

	}

}

void AMyCharacter::S2A_SpawnDecalAndEffect_Implementation(FHitResult OutHit, UParticleSystem * Hit, UMaterialInterface * DecalP)
{
	ACharacter* Pawn = Cast<ACharacter>(OutHit.GetActor());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Hit, OutHit.ImpactPoint + OutHit.ImpactNormal * 5.0f, OutHit.ImpactNormal.Rotation());
	UDecalComponent* Decal;
	if (Pawn)
	{
		Decal = UGameplayStatics::SpawnDecalAttached(DecalP, FVector(5, 5, 5), Pawn->GetMesh(), OutHit.BoneName, OutHit.ImpactPoint, OutHit.ImpactNormal.Rotation(), EAttachLocation::KeepWorldPosition, 5.0f);
	}
	else
	{
		Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalP, FVector(5, 5, 5), OutHit.ImpactPoint, OutHit.ImpactNormal.Rotation());
	}
	Decal->SetFadeScreenSize(0.001f);
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
	case EInteraction::Object:
		//UE_LOG(LogClass, Warning, TEXT("Object"));
		PickUp();
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
		Enemy->S2A_SetDie();
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
	AMyCharacter* CR = Cast<AMyCharacter>(DamageCauser);
	if (CR)
	{
		UE_LOG(LogClass, Warning, TEXT("Causer : %s(%s) to : %s(%s)"), *DamageCauser->GetName(), *CR->NickName, *this->GetName(), *NickName);
	}
	if (CurrentHP == 0)	//또 죽이지 않도록 방지
	{
		return 0;
	}
	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* RadialDamageEvent = (FRadialDamageEvent*)(&DamageEvent);
		if (RadialDamageEvent)
		{
			CurrentHP -= DamageAmount;
		}
		
	}
	else if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);
		if (PointDamageEvent->HitInfo.BoneName.Compare(TEXT("Head")) == 0)
		{
			CurrentHP = 0;
		}
		else
		{
			CurrentHP -= DamageAmount;
		}
	}
	else
	{
		CurrentHP -= DamageAmount;
	}

	FString HitName = FString::Printf(TEXT("Hit_%d"), FMath::RandRange(1, 4));
	PlayAnimMontage(HitAnimation, 1.0f, FName(*HitName));
	bIsReloading = false;
	CurrentHP = FMath::Clamp<float>(CurrentHP, 0, MaxHP);	//체력을 보정(-로 떨어져도 0으로 고정)

	if (CurrentHP <= 0)
	{
		S2A_SetDie();
		
		S2C_SetResultUI(DamageCauser, this);
		AGameGS* GS = Cast<AGameGS>(UGameplayStatics::GetGameState(GetWorld()));
		{
			GS->LeftAlive--;
		}
		AGameGM* GM = Cast<AGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GM)
		{
			/*UE_LOG(LogClass, Warning, TEXT("Check Aliver%s"), bIsAlive ? TEXT("Alive") : TEXT("Die"));*/
			GM->checkAliver();
		}
		else
		{
			UE_LOG(LogClass, Warning, TEXT("GM Not Load"));
		}
	}
	SetCurrentHPUI_OnRep();

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}



FVector AMyCharacter::GetSpringArmPosition() const
{
	return SpringArm->GetRelativeTransform().GetLocation();
}

void AMyCharacter::SetSpringArmPosition(FVector NewPosition)
{
	SpringArm->SetRelativeLocation(NewPosition);
}

void AMyCharacter::SetCurrentBulletUI()
{
	AGamePC* PC = GetController<AGamePC>();
	if (PC && PC->GameWidget)
	{
		PC->GameWidget->SetArmedBullet(CurrentBullet);
		PC->GameWidget->SetMagazine(Magazine);
	}
}


void AMyCharacter::SetCurrentHPUI_OnRep()
{
	AGamePC* PC = GetController<AGamePC>();

	if (PC)
	{
		if (CurrentHP == 0)
		{
			PC->SetHPBar(0);
		}
		else
		{
			PC->SetHPBar(CurrentHP / MaxHP);
		}
	}
}

void AMyCharacter::SetCurrentDegreeUI()
{
	AGamePC* PC = GetController<AGamePC>();
	//UE_LOG(LogClass, Warning, TEXT("%s"), *GetActorForwardVector().ToString());
	if (PC && PC->GameWidget)	//일단 임시로 이렇게 만들고, 추후에 개선해보자.
	{
		if (GetActorForwardVector().Y > 0)
		{
			PC->GameWidget->SetLocationDegree(90 - GetActorForwardVector().X * 90);
		}
		else
		{
			PC->GameWidget->SetLocationDegree(270 + GetActorForwardVector().X * 90);
		}
	}
}

void AMyCharacter::S2C_SetResultUI_Implementation(AActor* Causer, AActor* victim)
{
	AGamePC* PC =  Cast<AGamePC>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	AMyCharacter* CR = Cast<AMyCharacter>(Causer);
	if(PC && !bIsAlive && CR)
	{
		PC->ShowResult(bIsAlive);
		PC->bIsCurrentPlayerDie = true;
		AGamePS* PS = GetPlayerState<AGamePS>();
		if (PS)
		{
			FString SendLog = FString::Printf(TEXT("%s님이 %s님을 처치하였습니다."), *CR->NickName, *NickName);
			PC->C2S_SendMessage(FText::FromString(SendLog));
		}
	}
}

//void AMyCharacter::SetEndUI()
//{
//	UE_LOG(LogClass, Warning, TEXT("Result at PC - %s"), bIsGameDone ? TEXT("Done") : TEXT("Yet"));
//	UE_LOG(LogClass, Warning, TEXT("ShowResult at PC - %s"), bIsAlive ? TEXT("Alive") : TEXT("Die"));
//	AGamePC* PC = Cast<AGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
//	if (PC && !HasAuthority() && !bIsAlive)
//	{
//		PC->ShowResult(bIsAlive);
//	}
//}

void AMyCharacter::SetEndUI_Implementation()
{
	AGamePC* PC = Cast<AGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && bIsAlive)
	{
		PC->ShowResult(bIsAlive);
	}
}


void AMyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyCharacter, CurrentHP);
	DOREPLIFETIME(AMyCharacter, bIsSprint);
	DOREPLIFETIME(AMyCharacter, bIsAim);
	DOREPLIFETIME(AMyCharacter, bIsMotion);
	DOREPLIFETIME(AMyCharacter, bIsAlive);
	DOREPLIFETIME(AMyCharacter, bIsFire);
	DOREPLIFETIME(AMyCharacter, bIsReloading);
	DOREPLIFETIME(AMyCharacter, bIsStealthKill);
	DOREPLIFETIME(AMyCharacter, SprintSpeed);
	DOREPLIFETIME(AMyCharacter, WorldLocation);
	DOREPLIFETIME(AMyCharacter, WorldDirection);
	DOREPLIFETIME(AMyCharacter, TraceStart);
	DOREPLIFETIME(AMyCharacter, TraceEnd);
	DOREPLIFETIME(AMyCharacter, AimSpeed);
	DOREPLIFETIME(AMyCharacter, bIsGameDone);
}

void AMyCharacter::PickUp()
{
	//TraceObject();
	AMasterItem* Item = Cast<AMasterItem>(PickUpItem.GetActor());
	if (Item)
	{
		C2S_CheckPickUpItem(Item);
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("%s"), *PickUpItem.GetActor()->GetName());
	}
}

void AMyCharacter::C2S_CheckPickUpItem_Implementation(AMasterItem * Item)
{
	if (Item&& !Item->IsPendingKill())//IsPendingKill -> 서버상에서 삭제될 아이템. 
	{
		S2C_CompletePickUpItem(Item);
		Item->Destroy();
	}
}

void AMyCharacter::S2C_CompletePickUpItem_Implementation(AMasterItem * Item)
{
	if (Item && !Item->IsPendingKill())//IsPendingKill -> 서버상에서 삭제될 아이템. 
	{
		AGamePC* PC = Cast<AGamePC>(GetController());
		if (PC)
		{
			int32 SlotIndex = PC->InventoryWidget->GetEmptySlot();
			PC->InventoryWidget->SetItemSlot(SlotIndex, Item->ItemData);
		}
	}
}
