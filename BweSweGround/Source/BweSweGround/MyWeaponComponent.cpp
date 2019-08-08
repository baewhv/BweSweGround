// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeaponComponent.h"

UMyWeaponComponent::UMyWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMyWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMyWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
