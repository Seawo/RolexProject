// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/GS_TrainingRoom.h"

#include "GM_TrainingRoom.h"
#include "Kismet/GameplayStatics.h"
#include "Actor_FightPoint.h"

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
}

void AGS_TrainingRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (HasAuthority())
	{
		//DrawDebugString(GetWorld(), FVector(0, 0, 1000), FString::Printf(TEXT("PlayTime : %d"), (int32)GM->GetPlayTime()), nullptr, FColor::Red, DeltaTime);
		GM->SetPlayTime(DeltaTime);
		SetTimer(DeltaTime);
	}
	else
	{
		//DrawDebugString(GetWorld(), FVector(0, 0, 1000), FString::Printf(TEXT("PlayTime : %d"), (int32)GM->GetPlayTime()), nullptr, FColor::Red, DeltaTime);
	}
}

void AGS_TrainingRoom::SetTimer_Implementation(float DeltaTime)
{
	if (GM->GetPlayTime() >= 10.0f and not IsActiveBsePoint)
	{
		// 게임 시작하고 10초 후에 BasePoint 활성화
		int32 rand = FMath::RandRange(0, GM->Points.Num() - 1);
		UE_LOG(LogTemp, Warning, TEXT("rand : %d"), rand);

		GM->Points[rand]->SetActivePoint(EActivePoint::Active);
		UE_LOG(LogTemp, Warning, TEXT("BasePoint Active"));
		IsActiveBsePoint = true;
	}
}
