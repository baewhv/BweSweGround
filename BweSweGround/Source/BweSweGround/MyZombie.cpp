// Fill out your copyright notice in the Description page of Project Settings.


#include "MyZombie.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AMyZombie::AMyZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));*/

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88.0f), FRotator(0, -90.0f, 0));

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bIsAlive = true;
	Tags.Add(TEXT("Character"));
}

// Called when the game starts or when spawned
void AMyZombie::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHP = MaxHP;

}

// Called every frame
void AMyZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyZombie::Sprint_Start()
{
	bIsSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

}

void AMyZombie::Sprint_End()
{
	bIsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

float AMyZombie::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (CurrentHP == 0)
	{
		return 0;
	}
	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* RadialDamageEvent = (FRadialDamageEvent*)(&DamageEvent);
		if (RadialDamageEvent)
		{
			UE_LOG(LogClass, Warning, TEXT("Get RadialDamage"));
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
		UE_LOG(LogClass, Warning, TEXT("Get RadialDamage"));
	}
	

	CurrentHP = FMath::Clamp<float>(CurrentHP, 0, MaxHP);	//체력을 보정(-로 떨어져도 0으로 고정)

	if (CurrentHP == 0)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);		//콜리전 끄기

		CurrentState = EZombieState::Dead;
		SetLifeSpan(5.0f);
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AMyZombie::SetDamage(float damage)
{
}

