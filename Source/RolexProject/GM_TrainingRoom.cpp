// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_TrainingRoom.h"

#include "GS_TrainingRoom.h"
#include "PlayerController_TrainingRoom.h"

#include "Kismet/GameplayStatics.h"
#include "BaseCharacter.h"
#include "Actor_Point.h"
#include "Actor_FightPoint.h"

#include "Character_Phase.h"
#include "Character_Rampage.h"
#include "Character_Muriel.h"

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

	// 맵에 있는 거점 가져오기
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor_FightPoint::StaticClass(), foundActors);
	for (AActor* foundActor : foundActors)
	{
		AActor_FightPoint* fightPoint = Cast<AActor_FightPoint>(foundActor);
		fightPoint->OnPointOverlapChanged.AddDynamic(this, &AGM_TrainingRoom::ChangeNumberOfTeam);

		Points.Add(fightPoint);
	}
	PC = Cast<APlayerController_TrainingRoom>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// GameState의 변수 값 초기화
	GS = Cast<AGS_TrainingRoom>(UGameplayStatics::GetGameState(GetWorld()));

	if (GS)
	{
		GS->PlayTime = 0.0f;
		GS->IsActiveBsePoint = false;

		GS->ATeamChracters = ATeamChracters;
		GS->BTeamChracters = BTeamChracters;
		GS->Points = Points;

		GS->Occupation = EOccupation::None;
		GS->Result = EResult::None;
		GS->Clash = EClashing::None;

		GS->PointATeamCount = 0;
		GS->PointBTeamCount = 0;

		GS->PointTakeATeamGauge = 0;
		GS->PointTakeBTeamGauge = 0;

		GS->PointATeamGauge = 0;
		GS->PointBTeamGauge = 0;
		GS->PointATeamGaugePercent = 0;
		GS->PointBTeamGaugePercent = 0;

		GS->WaitTime = 0;

		GS->ExtraTime = 8.0f;
		GS->ExtraTimeDecrease = 1.0f;

		GS->IsGetATeamExtraTime = false;
		GS->IsGetBTeamExtraTime = false;
	}

}

void AGM_TrainingRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//DrawDebugString(GetWorld(), FVector(0, 0, 1000), FString::Printf(TEXT("PlayTime : %d"), (int32)PlayTime), nullptr, FColor::Red, DeltaTime);

	if (HasAuthority() == false)
	{
		return;
	}

	PlayTime += DeltaTime;


	if (PlayTime >= 10.0f)
	{
		if (IsActiveBsePoint == false)
		{
			int32 random = FMath::RandRange(0, Points.Num() - 1);

			Points[random]->SetActivePoint(EActivePoint::Active);
			IsActiveBsePoint = true;
		}


		UpdatePointGauge(DeltaTime);
		if (GS)
		{
			GS->PlayTime = PlayTime;
			GS->IsActiveBsePoint = IsActiveBsePoint;

			GS->Occupation = Occupation;
			GS->Result = Result;
			GS->Clash = Clash;

			GS->ATeamChracters = ATeamChracters;
			GS->BTeamChracters = BTeamChracters;
			GS->Points = Points;

			GS->PointATeamCount = PointATeamCount;
			GS->PointBTeamCount = PointBTeamCount;

			GS->PointTakeATeamGauge = PointTakeATeamGauge;
			GS->PointTakeBTeamGauge = PointTakeBTeamGauge;

			GS->PointATeamGauge = PointATeamGauge;
			GS->PointBTeamGauge = PointBTeamGauge;
			GS->PointATeamGaugePercent = PointATeamGaugePercent;
			GS->PointBTeamGaugePercent = PointBTeamGaugePercent;

			GS->WaitTime = WaitTime;

			GS->ExtraTime = ExtraTime;
			GS->ExtraTimeDecrease = ExtraTimeDecrease;
			GS->IsGetATeamExtraTime = IsGetATeamExtraTime;
			GS->IsGetBTeamExtraTime = IsGetBTeamExtraTime;
		}
		if (PC) // 서버의 UI값 업데이트
		{
			PC->SetPlayTime(PlayTime);
			PC->SetATeamCount(PointATeamCount);
			PC->SetBTeamCount(PointBTeamCount);
			PC->SetTakingGuage(PointTakeATeamGauge, PointTakeBTeamGauge);
			PC->SetPercent(Occupation, PointATeamGaugePercent, PointBTeamGaugePercent);
			PC->SetClashing(Clash);
			PC->SetExtraTime(ExtraTime);

			PC->SetIsATeamExtraTime(IsGetATeamExtraTime);
			PC->SetIsBTeamExtraTime(IsGetBTeamExtraTime);

		}
	}
	else
	{
		if (GS)
		{
			GS->PlayTime = PlayTime;
		}
		if (PC)
		{
			PC->SetPlayTime(PlayTime);
		}
	}
}

//APawn* AGM_TrainingRoom::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
//{
//	// 임시용으로 사용
//	UE_LOG(LogTemp, Warning, TEXT("SpawnDefaultPawnFor_Implementation"));
//	if (NewPlayer->IsLocalPlayerController()) // 서버라면
//	{
//		return GetWorld()->SpawnActor<ACharacter_Phase>(ACharacter_Phase::StaticClass(),StartSpot->GetActorLocation(), StartSpot->GetActorRotation());
//	}
//	else
//	{
//		return GetWorld()->SpawnActor<ACharacter_Muriel>(ACharacter_Muriel::StaticClass(), StartSpot->GetActorLocation(), StartSpot->GetActorRotation());
//	}
//}

void AGM_TrainingRoom::UpdatePointGauge(float DeltaTime)
{
	if (Occupation == EOccupation::None)
	{

		// 거점에 A팀만 있는 상태
		if (PointATeamCount > 0 and PointBTeamCount == 0)
		{
			WaitTime = 0;
			// B팀이 점령하기 위해서 거점에 먼저 올라가서 게이지가 조금이라도 차있는 상태라면?
			if (PointTakeBTeamGauge > 0)
			{
				// B팀 점령하기 위한 게이지를 차감하기
				PointTakeBTeamGauge -= 40 * DeltaTime;
				// 게이지가 0이 되면 0으로 고정
				if (PointTakeBTeamGauge <= 1.0f)
				{
					PointTakeBTeamGauge = 0.0f;
				}
			}
			// B팀이 점령하기 위한 게이지가 0이라면 
			else
			{
				// A팀이 점령하기 위한 게이지를 증가시킨다.
				PointTakeATeamGauge += 15 * PointATeamCount * DeltaTime;
				// 게이지가 100이 되면 점령하기 위한 게이지는 0으로 하고 거점상태를 A팀점령상태로 변경
				if (PointTakeATeamGauge >= 99.0f)
				{
					PointTakeATeamGauge = 0.0f;
					Occupation = EOccupation::TeamA;
				}

			}
		}
		// 거점에 B팀만 있는 경우
		else if (PointATeamCount == 0 and PointBTeamCount > 0)
		{
			WaitTime = 0;
			if (PointTakeATeamGauge > 0)
			{
				PointTakeATeamGauge -= 40 * DeltaTime;
				if (PointTakeATeamGauge <= 1.0f)
				{
					PointTakeATeamGauge = 0.0f;
				}
			}
			else
			{
				PointTakeBTeamGauge += 15 * PointBTeamCount * DeltaTime;

				if (PointTakeBTeamGauge >= 99.0f)
				{
					PointTakeBTeamGauge = 0.0f;
					Occupation = EOccupation::TeamB;
				}
			}
		}
		else if (PointATeamCount == 0 and PointBTeamCount == 0)
		{
			WaitTime += DeltaTime;
			if (WaitTime >= 3.0f)
			{
				PointTakeATeamGauge -= 50 * DeltaTime;
				PointTakeBTeamGauge -= 50 * DeltaTime;

				if (PointTakeATeamGauge <= 0)
				{
					PointTakeATeamGauge = 0.0f;
				}

				if (PointTakeBTeamGauge <= 0)
				{
					PointTakeBTeamGauge = 0.0f;
				}

				if (PointTakeATeamGauge == 0 and PointTakeBTeamGauge == 0)
				{
					WaitTime = 0.0f;
				}
			}
		}
	}
	else if (Occupation == EOccupation::TeamA)
	{
		if (PointBTeamCount > 0)
		{
			Clash = EClashing::Clash;
		}
		else
		{
			Clash = EClashing::None;
		}

		// 게이지가 다 차게되면 A팀 승리
		if (PointATeamGauge >= 120.0f)
		{
			// A팀 승리
			UE_LOG(LogTemp, Warning, TEXT("ATeam Win"));
			Result = EResult::AWin;
			return;
		}

		// 게이지값을 퍼센트로 변경
		PointATeamGaugePercent = PointATeamGauge / 120 * 100;

		// A팀 게이지가 90% 이상이고 B팀이 거점 안에 한번이라도 들어왔을 경우
		if (PointATeamGaugePercent >= 90.0f and PointBTeamCount > 0)
		{
			IsGetBTeamExtraTime = true;
		}

		// 추가 시간이 있을 경우
		if (IsGetBTeamExtraTime)
		{
			// A팀 게이지가 99.5% 이상이면
			if (PointATeamGaugePercent >= 99.0f)
			{
				// 추가시간이 끝났다면
				if (ExtraTime <= 0)
				{
					// A팀 게이지 다시 감소
					PointATeamGauge += DeltaTime;
				}
				// 추가시간이 끝나지 않았다면
				else
				{
					// A팀 게이지를 99.5%로 고정
					PointATeamGaugePercent = 99.5f;

					// B팀이 거점에 들어왔을 경우
					if (PointBTeamCount > 0)
					{
						ExtraTime = 8.0f;

						// 추가시간의 감소치를 증가시킨다.
						if (ExtraTimeDecrease > 4.0f)
						{
							ExtraTimeDecrease = 4.0f;
						}
						else
						{
							ExtraTimeDecrease += DeltaTime;
						}
					}
					else
					{
						// 추가 시간 차감
						ExtraTime -= ExtraTimeDecrease * DeltaTime;
					}
				}
			}
			// A팀의 게이지가 99.5%보다 낮다면 계속 게이지 증가
			else
			{
				PointATeamGauge += DeltaTime;
			}
		}
		// 추가 시간이 없을 경우
		else
		{
			PointATeamGauge += DeltaTime;
		}

		// A팀은 없고 B팀만 거점에 있을 경우
		// ->거점을 점령하기 위한게이지가 증가, 게이지가 100이 되면 B팀 거점으로 변경
		if (PointATeamCount == 0 and PointBTeamCount > 0)
		{
			PointTakeBTeamGauge += 15 * PointBTeamCount * DeltaTime;

			if (PointTakeBTeamGauge >= 100.0f)
			{
				PointTakeBTeamGauge = 0.0f;
				IsGetBTeamExtraTime = false;
				ExtraTimeDecrease = 0.0f;
				Clash = EClashing::None;
				Occupation = EOccupation::TeamB;
			}
		}

		// A팀과는 무관하게 B팀이 거점에 없을 경우
		// -> 거점 점령 대기시간이 줄어들면서 점령하기 위한 게이지가 줄어든다
		if (PointBTeamCount == 0)
		{
			WaitTime += DeltaTime;
			if (WaitTime >= 3.0f)
			{
				PointTakeBTeamGauge -= 50 * DeltaTime;
				if (PointTakeBTeamGauge <= 0)
				{
					PointTakeBTeamGauge = 0.0f;
					WaitTime = 0.0f;
				}
			}
		}
	}
	else if (Occupation == EOccupation::TeamB)
	{
		if (PointATeamCount > 0)
		{
			Clash = EClashing::Clash;
		}
		else
		{
			Clash = EClashing::None;
		}

		if (PointBTeamGauge >= 120.0f)
		{
			// B팀 승리
			UE_LOG(LogTemp, Warning, TEXT("BTeam Win"));
			Result = EResult::BWin;
			return;
		}

		PointBTeamGaugePercent = PointBTeamGauge / 120 * 100;
		// B팀 게이지가 90% 이상이고 A팀이 거점 안에 한번이라도 들어왔을 경우
		if (PointBTeamGaugePercent >= 90.0f and PointATeamCount > 0)
		{
			IsGetATeamExtraTime = true;
		}

		if (IsGetATeamExtraTime)
		{
			// B팀 게이지가 99.5% 이상이면
			if (PointBTeamGaugePercent >= 99.5f)
			{
				// 추가시간이 끝났다면
				if (ExtraTime <= 0)
				{
					// B팀 게이지 다시 감소
					PointBTeamGauge += DeltaTime;
				}
				// 추가시간이 끝나지 않았다면
				else
				{
					// B팀 게이지를 99.5%로 고정
					PointBTeamGaugePercent = 99.5f;
					// A팀이 거점에 들어왔을 경우
					if (PointATeamCount > 0)
					{
						ExtraTime = 8.0f;

						// 추가시간의 감소치를 증가시킨다.
						if (ExtraTimeDecrease > 4.0f)
						{
							ExtraTimeDecrease = 4.0f;
						}
						else
						{
							ExtraTimeDecrease += DeltaTime;
						}
					}
					else
					{
						// 추가 시간 차감
						ExtraTime -= ExtraTimeDecrease * DeltaTime;
					}
				}
			}
			// B팀의 게이지가 99.5%보다 낮다면 계속 게이지 증가
			else
			{
				PointBTeamGauge += DeltaTime;
			}
		}
		else
		{
			PointBTeamGauge += DeltaTime;
		}

		if (PointBTeamCount == 0 and PointATeamCount > 0)
		{
			PointTakeATeamGauge += 15 * PointATeamCount * DeltaTime;

			if (PointTakeATeamGauge >= 100.0f)
			{
				PointTakeATeamGauge = 0.0f;
				IsGetATeamExtraTime = false;
				ExtraTimeDecrease = 0.0f;
				Clash = EClashing::None;
				Occupation = EOccupation::TeamA;
			}
		}

		if (PointATeamCount == 0)
		{
			WaitTime += DeltaTime;
			if (WaitTime >= 3.0f)
			{
				PointTakeATeamGauge -= 50 * DeltaTime;
				if (PointTakeATeamGauge <= 0)
				{
					PointTakeATeamGauge = 0.0f;

					WaitTime = 0.0f;
				}
			}
		}
	}
}

void AGM_TrainingRoom::ChangeNumberOfTeam(bool bTeam, int32 ChangeValue)
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
