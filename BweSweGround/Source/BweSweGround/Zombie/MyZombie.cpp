// Fill out your copyright notice in the Description page of Project Settings.


#include "MyZombie.h"
#include "Zombie/AI/ZombieAIController.h"
#include "Player/MyCharacter.h"
#include "Game/ZombieHPBarWidgetBase.h"
#include "SpawnManager.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "UnrealNetwork.h"

// Sets default values
AMyZombie::AMyZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));*/

	HPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidget"));
	HPWidget->SetupAttachment(this->GetCapsuleComponent());

	//HPWidget->SetWidgetSpace(EWidgetSpace::Screen);
	
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88.0f), FRotator(0, -90.0f, 0));

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	Tags.Add(TEXT("Character"));
	Tags.Add(TEXT("Enemy"));

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AMyZombie::BeginPlay()
{
	Super::BeginPlay();

	HPWidget->SetRelativeLocation(FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2 - 50));

	PawnSensing->OnSeePawn.AddDynamic(this, &AMyZombie::OnSeenPawn);		//다른 컴포넌트의 델리게이트 호출방법(in cpp)
	PawnSensing->OnHearNoise.AddDynamic(this, &AMyZombie::OnHearedNoise);
	
	//C2S_initProperty();
}

// Called every frame
void AMyZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		PC->GetPlayerViewPoint(CameraLocation, CameraRotation);
		CameraRotation = FRotator(-CameraRotation.Pitch, CameraRotation.Yaw + 180.0f, CameraRotation.Roll);
		HPWidget->SetWorldRotation(CameraRotation);
		float Dist = FVector::Distance(CameraLocation, GetActorLocation());
		HPWidget->SetRelativeScale3D(FVector::OneVector * (Dist / 500.0f));
	}
}

// Called to bind functionality to input
void AMyZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMyZombie::C2S_initProperty_Implementation()
{
	CurrentHP = MaxHP;
	bIsAttack = false;
	bIsStealthKilled = false;
}

void AMyZombie::Sprint_Start()
{
	bIsSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	C2S_Sprint_Start();
}

void AMyZombie::C2S_Sprint_Start_Implementation()
{
	bIsSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AMyZombie::Sprint_End()
{
	bIsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	C2S_Sprint_End();
}

void AMyZombie::C2S_Sprint_End_Implementation()
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
	SetHPWidget_OnRep();

	CurrentHP = FMath::Clamp<float>(CurrentHP, 0, MaxHP);	//체력을 보정(-로 떨어져도 0으로 고정)

	if (CurrentHP == 0)
	{
		S2A_SetDie();
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AMyZombie::S2A_SetDie_Implementation()
{
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);		//콜리전 끄기
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("DeadBody"));


	CurrentState = EZombieState::Dead;

	AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
	if (AIC)
	{
		AIC->SetCurrentState(CurrentState);
	}
}

void AMyZombie::SetHPWidget_OnRep()
{
	UZombieHPBarWidgetBase* SetZombieHpWidget = Cast<UZombieHPBarWidgetBase>(HPWidget->GetUserWidgetObject());
	if (SetZombieHpWidget)
	{
		UE_LOG(LogClass, Warning, TEXT("SetHPWidget_OnRep"));
		SetZombieHpWidget->SetZombieHP(CurrentHP/MaxHP);
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("Fail!"));
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

void AMyZombie::getSpawnPoint(ASpawnManager * point)
{
	SpawnPoint = point;
}

void AMyZombie::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyZombie, CurrentHP);
	DOREPLIFETIME(AMyZombie, bIsSprint);
	DOREPLIFETIME(AMyZombie, bIsAttack);
	DOREPLIFETIME(AMyZombie, CurrentState);

}
