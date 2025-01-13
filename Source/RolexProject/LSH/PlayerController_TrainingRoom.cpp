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

void APlayerController_TrainingRoom::SetPlayTime(float Time)
{
	if (IsLocalController() and UI_Zone)
	{
		UI_Zone->SetPlayTime(Time);
	}
}

void APlayerController_TrainingRoom::SetTakingGuage(float Agauge, float Bgauge) // 0 ~ 100
{
	if (IsLocalController() and UI_Zone)
	{
		float A = (int)Agauge / 100.0f;
		float B = (int)Bgauge / 100.0f;
		UI_Zone->SetTakingGuage(A, B);
	}
}

void APlayerController_TrainingRoom::SetATeamCount(int32 Count)
{
	if (IsLocalController() and UI_Zone)
	{
		UI_Zone->SetATeamCount(Count);
	}
}

void APlayerController_TrainingRoom::SetBTeamCount(int32 Count)
{
	if (IsLocalController() and UI_Zone)
	{
		UI_Zone->SetBTeamCount(Count);
	}
}

void APlayerController_TrainingRoom::SetPercent(EOccupation occupation ,float APercent, float BPercent)
{
	if (IsLocalController() and UI_Zone)
	{
		UI_Zone->SetPercent(occupation, APercent, BPercent);
	}
}

void APlayerController_TrainingRoom::SetClashing(EClashing clash)
{
	if (IsLocalController() and UI_Zone)
	{
		UI_Zone->SetClashing(clash);
	}
}

void APlayerController_TrainingRoom::SetExtraTime(float Time)
{
	if (IsLocalController() and UI_Zone)
	{
		UI_Zone->SetExtraTime(Time);
	}
}

void APlayerController_TrainingRoom::SetIsATeamExtraTime(bool bExtra)
{
	if (IsLocalController() and UI_Zone)
	{
		UI_Zone->SetIsATeamExtraTime(bExtra);
	}
}

void APlayerController_TrainingRoom::SetIsBTeamExtraTime(bool bExtra)
{
	if (IsLocalController() and UI_Zone)
	{
		UI_Zone->SetIsBTeamExtraTime(bExtra);
	}
}
