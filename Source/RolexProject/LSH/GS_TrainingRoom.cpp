// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/GS_TrainingRoom.h"

#include "GM_TrainingRoom.h"
#include "Kismet/GameplayStatics.h"
#include "Actor_FightPoint.h"
#include "BaseCharacter.h"

#include "Net/UnrealNetwork.h"

AGS_TrainingRoom::AGS_TrainingRoom()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}

void AGS_TrainingRoom::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("GameState initialized on SERVER"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameState initialized on CLIENT"));
	}
}

void AGS_TrainingRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGS_TrainingRoom::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGS_TrainingRoom, PlayTime);
	DOREPLIFETIME(AGS_TrainingRoom, Points);

	DOREPLIFETIME(AGS_TrainingRoom, Occupation);
	DOREPLIFETIME(AGS_TrainingRoom, Result);
	DOREPLIFETIME(AGS_TrainingRoom, Clash);

	DOREPLIFETIME(AGS_TrainingRoom, PointATeamCount);
	DOREPLIFETIME(AGS_TrainingRoom, PointBTeamCount);

	DOREPLIFETIME(AGS_TrainingRoom, PointTakeATeamGauge);
	DOREPLIFETIME(AGS_TrainingRoom, PointTakeBTeamGauge);

	DOREPLIFETIME(AGS_TrainingRoom, PointATeamGauge);
	DOREPLIFETIME(AGS_TrainingRoom, PointBTeamGauge);
	DOREPLIFETIME(AGS_TrainingRoom, PointATeamGaugePercent);
	DOREPLIFETIME(AGS_TrainingRoom, PointBTeamGaugePercent);

	DOREPLIFETIME(AGS_TrainingRoom, WaitTime);
	DOREPLIFETIME(AGS_TrainingRoom, ExtraTime);
	DOREPLIFETIME(AGS_TrainingRoom, ExtraTimeDecrease);

	DOREPLIFETIME(AGS_TrainingRoom, IsGetATeamExtraTime);
	DOREPLIFETIME(AGS_TrainingRoom, IsGetBTeamExtraTime);
}

void AGS_TrainingRoom::OnRep_PlayTime()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("[GS_OnRep_Server] PlayTime"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[GS_OnRep_Client] PlayTime"));
	}
}

void AGS_TrainingRoom::OnRep_Points()
{
	UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] Points"));
}

void AGS_TrainingRoom::OnRep_Occupation()
{
	UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] Occupation"));
}

void AGS_TrainingRoom::OnRep_Result()
{
	UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] Result"));
}

void AGS_TrainingRoom::OnRep_Clash()
{
	UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] Clash"));
}

void AGS_TrainingRoom::OnRep_PointATeamCount()
{
	UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] PointATeamCount"));
}

void AGS_TrainingRoom::OnRep_PointBTeamCount()
{
	UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] PointBTeamCount"));
}

void AGS_TrainingRoom::OnRep_PointTakeATeamGauge()
{
	UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] PointTakeATeamGauge"));
}

void AGS_TrainingRoom::OnRep_PointTakeBTeamGauge()
{
	UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] PointTakeBTeamGauge"));
}

void AGS_TrainingRoom::OnRep_PointATeamGauge()
{
	UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] PointATeamGauge"));
}

void AGS_TrainingRoom::OnRep_PointBTeamGauge()
{
	UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] PointBTeamGauge"));
}

void AGS_TrainingRoom::OnRep_PointATeamGaugePercent()
{
	UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] PointATeamGaugePercent"));
}

void AGS_TrainingRoom::OnRep_PointBTeamGaugePercent()
{
	UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] PointBTeamGaugePercent"));
}

void AGS_TrainingRoom::OnRep_WaitTime()
{
	UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] WaitTime"));
}

void AGS_TrainingRoom::OnRep_ExtraTime()
{
	UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] ExtraTime"));
}

void AGS_TrainingRoom::OnRep_ExtraTimeDecrease()
{
	UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] ExtraTimeDecrease"));
}

void AGS_TrainingRoom::OnRep_IsGetATeamExtraTime()
{
	UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] IsGetATeamExtraTime"))
}

void AGS_TrainingRoom::OnRep_IsGetBTeamExtraTime()
{
	UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] IsGetBTeamExtraTime"))
}