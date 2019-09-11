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
#include "Kismet/KismetMathLibrary.h"
#include "SpawnAgent.h"
#include "Animation/AnimInstance.h"

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
	Tags.Add(TEXT("Enemy"));

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	

}

// Called when the game starts or when spawned
void AMyZombie::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHP = MaxHP;
	bIsAttack = false;
	bIsStealthKilled = false;
	PawnSensing->OnSeePawn.AddDynamic(this, &AMyZombie::OnSeenPawn);		//다른 컴포넌트의 델리게이트 호출방법(in cpp)
	PawnSensing->OnHearNoise.AddDynamic(this, &AMyZombie::OnHearedNoise);

}

// Called every frame
void AMyZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogClass, Warning, TEXT("Dot"));
	//AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
	//if (AIC)
	//{
	//	AActor* Player = Cast<AActor>(AIC->BBComponent->GetValueAsObject(FName(TEXT("Player"))));
	//	if (Player)
	//	{
	//		//float Distance = FVector::Distance(GetActorLocation(), Player->GetActorLocation());
	//		//FVector PlayerVector = Player->GetActorLocation() - GetActorLocation();
	//		//float aa = UKismetMathLibrary::DotProduct2D(FrontVector, PlayerVector);
	//		;
	//		float Dot = GetMesh()->GetAnimInstance()->CalculateDirection((Player->GetActorLocation() - GetActorLocation()).GetSafeNormal(), GetActorRotation());
	//		/*if (Distance < 200.0f && Dot > 0.0f)
	//		{
	//			UGameplayStatics::ApplyDamage(Player, 30.0f, GetController(), this, nullptr);
	//		}*/
	//		UE_LOG(LogClass, Warning, TEXT("Dot : %f"), UKismetMathLibrary::Abs(Dot/180.0f));
	//	}
	//}
	
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
		UE_LOG(LogClass, Warning, TEXT("Get Damage"));
	}
	if (!bIsAttack)
	{
		FString HitName = FString::Printf(TEXT("Hit%d"), FMath::RandRange(1, 2));
		PlayAnimMontage(HitAnimation, 1.0f, FName(*HitName));
	}

	CurrentHP = FMath::Clamp<float>(CurrentHP, 0, MaxHP);	//체력을 보정(-로 떨어져도 0으로 고정)

	if (CurrentHP == 0)
	{
		SetDie();
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AMyZombie::SetDie()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);		//콜리전 끄기

	CurrentState = EZombieState::Dead;

	AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
	if (AIC)
	{
		AIC->SetCurrentState(CurrentState);
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
			float Distance = FVector::Distance(GetActorLocation(), Player->GetActorLocation());
			float Dot = GetMesh()->GetAnimInstance()->CalculateDirection(Player->GetActorLocation() - GetActorLocation(), GetActorRotation());
			if (Distance < 200.0f && UKismetMathLibrary::Abs(Dot) < 90.0f)
			{
				UGameplayStatics::ApplyDamage(Player, 30.0f, GetController(), this, nullptr);
			}
		}
	}
}


void AMyZombie::OnSeenPawn(APawn * PP)
{
	AMyCharacter* Player = Cast<AMyCharacter>(PP);
	if (!Player || CurrentState == EZombieState::Dead)
	{
		return;
	}

	//UE_LOG(LogClass, Warning, TEXT("Active"));
	if ((CurrentState == EZombieState::Normal || CurrentState == EZombieState::Alert) && PP->ActorHasTag(TEXT("Player")) && Player->CurrentHP > 0)
	{
		CurrentState = EZombieState::Chase;
		AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
		if (AIC)
		{
			AIC->SetCurrentState(CurrentState);
			AIC->SetTargetPlayer(PP);
		}
	}
}

void AMyZombie::OnHearedNoise(APawn * pawn, const FVector & Location, float Volume)
{
	if (CurrentState == EZombieState::Normal)
	{
		UE_LOG(LogClass, Warning, TEXT("%.0f,%.0f,%.0f"), Location.X, Location.Y, Location.Z);
		CurrentState = EZombieState::Alert;
		AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
		if (AIC)
		{
			AIC->SetCurrentState(CurrentState);
			AIC->SetTargetLocation(Location);
		}
	}
}

//소리 들은 후 순서
//alert(turn) -> move -> normal

void AEnemy::getSpawnPoint(ASpawnAgent * point)
{
	SpawnPoint = point;
}