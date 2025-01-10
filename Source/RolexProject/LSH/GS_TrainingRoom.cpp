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

	GM = Cast<AGM_TrainingRoom>(UGameplayStatics::GetGameMode(GetWorld()));
}

void AGS_TrainingRoom::BeginPlay()
{
	Super::BeginPlay();
	
	IsActiveBsePoint = false;
	TArray<AActor*> foundActors;
	// 월드상의 모든 캐릭터 탐색
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), foundActors);
	for (AActor* foundActor : foundActors)
	{
		ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(foundActor);
		if (baseCharacter->Data.Team == true)
		{
			ATeamChracters.Add(baseCharacter);
		}
		else
		{
			BTeamChracters.Add(baseCharacter);
		}
	}

	// 월드상의 모든 거점 탐색
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor_FightPoint::StaticClass(), foundActors);
	for (AActor* foundActor : foundActors)
	{
		AActor_FightPoint* fightPoint = Cast<AActor_FightPoint>(foundActor);

		// 거점의 델리게이트 함수에 대리자 추가
		fightPoint->OnPointOverlapChanged.AddDynamic(this, &AGS_TrainingRoom::ChangeNumberOfTeam);

		Points.Add(fightPoint);
	}
}

void AGS_TrainingRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGS_TrainingRoom::ChangeNumberOfTeam(bool bTeam, int32 ChangeValue)
{
	if (bTeam)
	{
		PointATeamCount += ChangeValue;
	}
	else
	{
		PointBTeamCount += ChangeValue;
	}
}


void AGS_TrainingRoom::NoneOccupation()
{
}

void AGS_TrainingRoom::ATeamOccupation()
{
}

void AGS_TrainingRoom::BTeamOccupation()
{
}
