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

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMyCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AMyCharacter::StopJumping);
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


FRotator AMyCharacter::GetAimOffset() const
{
	//FVector AimDirWS = GetBaseAimRotation().Vector();
	//FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);	//월드 방향을 로컬방향으로 전환.
	//FRotator AimRotLS = AimDirLS.Rotation();

	return ActorToWorld().InverseTransformVectorNoScale(GetBaseAimRotation().Vector()).Rotation();
}

