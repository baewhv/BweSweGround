// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCameraShake.h"

UMyCameraShake::UMyCameraShake()
{
	OscillationDuration = 0.12f;	//위 아래로 흔듦.
	OscillationBlendInTime = 0.05; //위 아래로 부드럽게 흔들 정도.
	OscillationBlendOutTime = 0.05; 

	RotOscillation.Pitch.Amplitude = 3.0f;	//카메라를 회전하듯 튕길 정도.
	RotOscillation.Pitch.Frequency = 1;	//?
	RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;	//?
}
