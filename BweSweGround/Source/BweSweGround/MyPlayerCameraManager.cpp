// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerCameraManager.h"
//#include "Kismet/GameplayStatics.h"
#include "MyPlayerController.h"
#include "MyCharacter.h"

void AMyPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);

	//UGameplayStatics::GetPlayerPawn(GetWorld(), );	폰을 불러오는 간단한 방법.

	//GetOwner
	AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwningPlayerController());
	if (PC)
	{
		AMyCharacter* Pawn = Cast<AMyCharacter>(PC->GetPawn());
		if (Pawn)
		{
			float TargetFOV = Pawn->bIsAim ? IronsightFOV : NormalFOV;
			SetFOV(FMath::FInterpTo(GetFOVAngle(), TargetFOV, DeltaTime, InterpSpeed));	//서서히 줌인 또는 줌아웃이 되는 효과.
			//FMath::RInterpTo	//회전용
			//FMath::QInterpTo	//Quat?
			//FMath::VInterpTo	//벡터용

			
			/*float CrouchedZ = Pawn->bIsCrouched ? CrouchingZ : NormalZ;
			GetCameraLocation().Z - CrouchedZ;*/
			//ApplyWorldOffset(GetCameraLocation() - FVector(0, 0, Pawn->bIsCrouched ? CrouchingZ : NormalZ), false);
			
			FVector TargetPos = Pawn->bIsCrouched ? Pawn->CrouchSpringArmPosition : Pawn->NormalSpringArmPosition;
			Pawn->SetSpringArmPosition(FMath::VInterpTo(Pawn->GetSpringArmPosition(), TargetPos, DeltaTime, 10.0f));
		}
	}
}
