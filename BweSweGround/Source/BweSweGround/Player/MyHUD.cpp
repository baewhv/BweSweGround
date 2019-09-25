// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Engine/Canvas.h"
#include "Engine/StreamableManager.h"
#include "Engine/Texture2D.h"
#include "Player/MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Item/ItemTable.h"

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();
	FStreamableManager LoadAsset;

	FSoftObjectPath CrosshairAsset(TEXT("Texture2D'/Game/Data/crosshair.crosshair'"));
	Crosshair = Cast<UTexture2D>(LoadAsset.LoadSynchronous(CrosshairAsset));

}

void AMyHUD::DrawHUD()
{
	/*Canvas->K2_DrawLine(FVector2D(Canvas->SizeX/2 - 2, Canvas->SizeY/2), FVector2D(Canvas->SizeX/2 + 2, Canvas->SizeY/2), 1.0f, FLinearColor::Red);
	Canvas->K2_DrawLine(FVector2D(Canvas->SizeX/2, Canvas->SizeY/2+2), FVector2D(Canvas->SizeX/2, Canvas->SizeY/2-2), 1.0f, FLinearColor::Red);*/

	int Ruler = Canvas->SizeX / 20.0f;	//캔버스 화면비에 맞게 그리기

	//DrawLine(Canvas->SizeX / 2 - Ruler, Canvas->SizeY / 2, Canvas->SizeX / 2 + Ruler, Canvas->SizeY / 2, FColor::Red);
	//DrawLine(Canvas->SizeX / 2, Canvas->SizeY / 2 - Ruler, Canvas->SizeX / 2, Canvas->SizeY / 2 + Ruler, FColor::Red);
	
	//AMyCharacter* Player = Cast<AMyCharacter>(GetOwner());
	
	AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Player)
	{
		//FString string = FString::Printf(TEXT("HP : %.f\nAmmo : %d"), Player->CurrentHP, Player->CurrentBullet);
		//DrawText(string, FLinearColor::Blue, Canvas->SizeX/2 , Canvas->SizeY / 2);
		
		if (Player->InteractionType == EInteraction::Stealth)
		{
			FString string_stealth = FString::Printf(TEXT("[E]StealthKill"));
			DrawText(string_stealth, FLinearColor::Blue, (Canvas->SizeX / 2) - string_stealth.Len() * 5.5f, Canvas->SizeY / 2);
			//DrawText(string, FLinearColor::Blue, Canvas->SizeX / 2, Canvas->SizeY / 2);
		}
		else if (Player->InteractionType == EInteraction::Object)
		{
			//FItemTable Item = Cast<FItemTable>(Player->InteractTarget);
			/*if (Item)
			{
				FString string_object = FString::Printf(TEXT("[E]get[%s]"), *Item.ItemName);
				DrawText(string_object, FLinearColor::Blue, (Canvas->SizeX / 2) - string_object.Len() * 5.5f, Canvas->SizeY / 2);
			}*/
		}
		/*else
		{
			string_stealth = FString::Printf(TEXT(""));
		}*/
		
		
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("Get Player Component Fail"));
	}


	if (Crosshair)
	{
		DrawTextureSimple(Crosshair, Canvas->SizeX / 2 - (Crosshair->GetSizeX() / 2), Canvas->SizeY / 2 - (Crosshair->GetSizeY() / 2));
	}
}
