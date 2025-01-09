// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Actor_FightPoint.h"

#include "BaseCharacter.h"

// Sets default values
AActor_FightPoint::AActor_FightPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActor_FightPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActor_FightPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0,0, 500), FString::Printf(TEXT("ActivePoint : %d"), ActivePoint), nullptr, FColor::Yellow, DeltaTime);

	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0,0, 450), FString::Printf(TEXT("TakePointGauge : %.2f"), TakePointATeamGauge), nullptr, FColor::Red, DeltaTime);
	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0,0, 400), FString::Printf(TEXT("TakePointGauge : %.2f"), TakePointBTeamGauge), nullptr, FColor::Blue, DeltaTime);



	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0,0, 300), FString::Printf(TEXT("Team : %d"), Team), nullptr, FColor::Green, DeltaTime);

	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0,0, 250), FString::Printf(TEXT("ATeamPointGauge : %.2f"), ATeamPointGauge), nullptr, FColor::Red, DeltaTime);
	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0,0, 200), FString::Printf(TEXT("ActiveATeamCount : %d"), ActiveATeamCount), nullptr, FColor::Red, DeltaTime);

	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0,0, 100), FString::Printf(TEXT("BTeamPointGauge : %.2f"), BTeamPointGauge), nullptr, FColor::Blue, DeltaTime);
	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0,0, 50), FString::Printf(TEXT("ActiveBTeamCount : %d"), ActiveBTeamCount), nullptr, FColor::Blue, DeltaTime);


	SetPointGauge(DeltaTime);
}

void AActor_FightPoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActivePoint == EActivePoint::Deactivate) return;

	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UE_LOG(LogTemp, Warning, TEXT("[FightPoint] OnOverlapBegin"));

	ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);
	if (character and character->Data.Team == true)
	{
		ActiveATeamCount++;
	}
	else if (character and character->Data.Team == false)
	{
		ActiveBTeamCount++;
	}


	//SetTeam();
}

void AActor_FightPoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ActivePoint == EActivePoint::Deactivate) return;



	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	UE_LOG(LogTemp, Warning, TEXT("[FightPoint] OnOverlapEnd"));

	ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);
	if (character and character->Data.Team == true)
	{
		ActiveATeamCount--;
	}
	else if (character and character->Data.Team == false)
	{
		ActiveBTeamCount--;
	}

	//SetTeam();
}

// 상대 거점이거나, 아무도 거점을 먹지 았을 경우의 게이지 업데이트 함수
void AActor_FightPoint::UpdateTakePointGauge(float DeltaTime)
{
}

// 각 팀 거점 게이지 업데이트 함수
void AActor_FightPoint::UpdateTeamPointGauge(float DeltaTime)
{
	switch (Team)
	{
	case ETeam::None:
		break;
	case ETeam::TeamA:
		ATeamPointGauge += 2 * DeltaTime;
		break;
	case ETeam::TeamB:
		BTeamPointGauge += 2 * DeltaTime;
		break;
	case ETeam::Clashing:
		break;
	default:
		break;
	}
}

void AActor_FightPoint::SetPointGauge(float DeltaTime)
{
	if (Team == ETeam::None)
	{
		// 거점에 A팀만 있는 상태
		if (ActiveATeamCount > 0 and ActiveBTeamCount == 0)
		{
			if (TakePointBTeamGauge > 0)
			{
				TakePointBTeamGauge -= 40 * DeltaTime;
				if (TakePointBTeamGauge <= 0)
				{
					TakePointBTeamGauge = 0.0f;
				}
			}
			else
			{
				TakePointATeamGauge += 15 * ActiveATeamCount * DeltaTime;
				if (TakePointATeamGauge >= 100.0f)
				{
					TakePointATeamGauge = 0.0f;
					Team = ETeam::TeamA;
				}
				
			}
		}
		// 거점에 B팀만 있는 경우
		else if (ActiveATeamCount == 0 and ActiveBTeamCount > 0)
		{
			if (TakePointATeamGauge > 0)
			{
				TakePointATeamGauge -= 40 * DeltaTime;
				if (TakePointATeamGauge <= 0)
				{
					TakePointATeamGauge = 0.0f;
				}
			}
			else
			{
				TakePointBTeamGauge += 15 * ActiveBTeamCount * DeltaTime;

				if (TakePointBTeamGauge >= 100.0f)
				{
					TakePointBTeamGauge = 0.0f;
					Team = ETeam::TeamB;
				}
			}
		}
	}
	else if (Team == ETeam::TeamA)
	{
		if (ATeamPointGauge >= 120.0f)
		{
			// A팀 승리
			UE_LOG(LogTemp, Warning, TEXT("ATeam Win"));
			return;
		}

		ATeamPointGauge += DeltaTime;

		if (ActiveATeamCount == 0 and ActiveBTeamCount > 0)
		{
			TakePointBTeamGauge += 15 * ActiveBTeamCount *  DeltaTime;

			if (TakePointBTeamGauge >= 100.0f)
			{
				TakePointBTeamGauge = 0.0f;
				Team = ETeam::TeamB;
			}
		}

		if (ActiveBTeamCount == 0)
		{
			Timer += DeltaTime;
			if (Timer >= 3.0f)
			{
				TakePointBTeamGauge -= 50 * DeltaTime;
				if (TakePointBTeamGauge <= 0)
				{
					TakePointBTeamGauge = 0.0f;
					Timer = 0.0f;
				}
			}
		}

		
	}
	else if (Team == ETeam::TeamB)
	{

		if (BTeamPointGauge >= 120.0f)
		{
			// B팀 승리
			UE_LOG(LogTemp, Warning, TEXT("BTeam Win"));
			return;
		}

		BTeamPointGauge += DeltaTime;

		if (ActiveBTeamCount == 0 and ActiveATeamCount > 0)
		{
			TakePointATeamGauge += 15 * ActiveATeamCount * DeltaTime;

			if (TakePointATeamGauge >= 100.0f)
			{
				TakePointATeamGauge = 0.0f;
				Team = ETeam::TeamA;
			}
		}

		if (ActiveATeamCount == 0)
		{
			Timer += DeltaTime;
			if (Timer >= 3.0f)
			{
				TakePointATeamGauge -= 50 * DeltaTime;
				if (TakePointATeamGauge <= 0)
				{
					TakePointATeamGauge = 0.0f;
					Timer = 0.0f;
				}
			}
		}
	}
	else if (Team == ETeam::Clashing)
	{
	}


	switch (Team)
	{
	case ETeam::None:
		break;
	case ETeam::TeamA:
		break;
	case ETeam::TeamB:
		break;
	case ETeam::Clashing:
		break;
	default:
		break;
	}

	//if (ActiveATema > 0 and ActiveBTema == 0)
	//{
	//	Team = ETeam::TeamA;
	//}
	//else if (ActiveATema == 0 and ActiveBTema > 0)
	//{
	//	Team = ETeam::TeamB;
	//}
	//else if (ActiveATema > 0 and ActiveBTema > 0)
	//{
	//	Team = ETeam::Clashing;
	//}
	//else
	//{
	//	Team = ETeam::None;
	//}
}

