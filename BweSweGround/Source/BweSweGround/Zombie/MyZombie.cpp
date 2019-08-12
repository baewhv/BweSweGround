// Fill out your copyright notice in the Description page of Project Settings.


#include "MyZombie.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Zombie/AI/ZombieAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/MyCharacter.h"

// Sets default values
AMyZombie::AMyZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));*/

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88.0f), FRotator(0, -90.0f, 0));

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	Tags.Add(TEXT("Character"));

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	

}

// Called when the game starts or when spawned
void AMyZombie::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHP = MaxHP;

	PawnSensing->OnSeePawn.AddDynamic(this, &AMyZombie::OnSeenPawn);		//다른 컴포넌트의 델리게이트 호출방법(in cpp)
	//PawnSensing->OnHearNoise.AddDynamic(this, &AMyZombie::OnHearedNoise);

}

// Called every frame
void AMyZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogClass, Warning, TEXT("%d"), PawnSensing->bSeePawns);
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
	
	FString HitName = FString::Printf(TEXT("Hit%d"), FMath::RandRange(1, 2));
	PlayAnimMontage(HitAnimation, 1.0f, FName(*HitName));

	CurrentHP = FMath::Clamp<float>(CurrentHP, 0, MaxHP);	//체력을 보정(-로 떨어져도 0으로 고정)

	if (CurrentHP == 0)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);		//콜리전 끄기

		CurrentState = EZombieState::Dead;

		AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
		if (AIC)
		{
			AIC->SetCurrentState(CurrentState);
		}
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}


void AMyZombie::OnSeenPawn(APawn * Pawn)
{
	AMyCharacter* Player = Cast<AMyCharacter>(Pawn);
	if (!Player)
	{
		return;
	}

	//UE_LOG(LogClass, Warning, TEXT("Active"));
	if (CurrentState == EZombieState::Normal && Pawn->ActorHasTag(TEXT("Player"))&& Player->CurrentHP>0)
	{
		CurrentState = EZombieState::Chase;
		AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
		if (AIC)
		{
			AIC->SetCurrentState(CurrentState);
			AIC->SetTargetPlayer(Pawn);
		}
	}
}

void AMyZombie::Attack()
{
	AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
	if (AIC)
	{
		AActor* Player = Cast<AActor>(AIC->BBComponent->GetValueAsObject(FName(TEXT("Player"))));
		if (Player)
		{
			UGameplayStatics::ApplyDamage(Player, 30.0f, GetController(), this, nullptr);
		}
	}
}

//void AMyZombie::OnHearedNoise(APawn * Pawn, const FVector & Location, float Volume)
//{
//}

