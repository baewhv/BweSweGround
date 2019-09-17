// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

FString UMyGameInstance::GetUserID()
{
	return UserID.IsEmpty() ? FString::Printf(TEXT("Noname%d"), FMath::RandRange(0, 100)) : UserID;
}
