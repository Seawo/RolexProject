// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/PlayerController_TrainingRoom.h"

#include "LSH/UI/UI_Zone.h"
#include "GS_TrainingRoom.h"

#include "Kismet/GameplayStatics.h"


void APlayerController_TrainingRoom::BeginPlay()
{
	Super::BeginPlay();

	if (UI_ZoneClass)
	{
		UI_Zone = CreateWidget<UUI_Zone>(this, UI_ZoneClass);
		if (UI_Zone)
		{
			UI_Zone->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UI_Zone is nullptr"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UI_ZoneClass is nullptr"));
	}

	InitUI();
}

void APlayerController_TrainingRoom::InitUI()
{
	if(IsLocalController() == false)
	{
		return;
	}


	AGS_TrainingRoom* GS = GetWorld()->GetGameState<AGS_TrainingRoom>();

	if (GS)
	{
		UI_Zone->UIInit();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GS is nullptr"));
	}
}
