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

	IsActiveBsePoint = false;

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

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor_FightPoint::StaticClass(), foundActors);
	for (AActor* foundActor : foundActors)
	{
		AActor_FightPoint* fightPoint = Cast<AActor_FightPoint>(foundActor);
		
		Points.Add(fightPoint);
	}
}

void AGM_TrainingRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//DrawDebugString(GetWorld(), FVector(0, 0, 1000), FString::Printf(TEXT("PlayTime : %d"), (int32)PlayTime), nullptr, FColor::Red, DeltaTime);

}
