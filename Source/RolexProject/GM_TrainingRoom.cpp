// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_TrainingRoom.h"

#include "Kismet/GameplayStatics.h"
#include "BaseCharacter.h"
#include "Actor_Point.h"
#include "Actor_FightPoint.h"

AGM_TrainingRoom::AGM_TrainingRoom()
{
	PrimaryActorTick.bCanEverTick = true;


}

void AGM_TrainingRoom::BeginPlay()
{
	Super::BeginPlay();

	// 각 팀 캐릭터 값 가져오기
	TArray<AActor*> foundActors;
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

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor_Point::StaticClass(), foundActors);
	for (AActor* foundActor : foundActors)
	{
		AActor_FightPoint* fightPoint = Cast<AActor_FightPoint>(foundActor);
		
		Points.Add(fightPoint);
	}
}

void AGM_TrainingRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayTime += DeltaTime;

	static bool isActiveBsePoint = false;

	if (PlayTime >= 10.0f and not isActiveBsePoint)
	{
		// 게임 시작하고 10초 후에 BasePoint 활성화
		int32 rand = FMath::RandRange(0,Points.Num() -1);

		Points[rand]->SetActivePoint(EActivePoint::Active);
		UE_LOG(LogTemp, Warning, TEXT("BasePoint Active"));
		isActiveBsePoint = true;
	}

	DrawDebugString(GetWorld(), FVector(0, 0, 1000), FString::Printf(TEXT("PlayTime : %d"), (int32)PlayTime), nullptr, FColor::Red, DeltaTime);
}
