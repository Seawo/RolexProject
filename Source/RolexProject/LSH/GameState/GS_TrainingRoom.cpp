// Fill out your copyright notice in the Description page of Project Settings.


#include "GS_TrainingRoom.h"

#include "PlayerController/PlayerController_TrainingRoom.h"
#include "RolexPlayerController.h"
#include "RolexGameInstance.h"
#include "GM_TrainingRoom.h"
#include "Kismet/GameplayStatics.h"
#include "Point/Actor_FightPoint.h"
#include "BaseCharacter.h"
#include "UI_Zone.h"
#include "Point/Actor_StartDoor.h"

#include "Net/UnrealNetwork.h"

AGS_TrainingRoom::AGS_TrainingRoom()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}

void AGS_TrainingRoom::BeginPlay()
{
	Super::BeginPlay();

	IsActiveBsePoint = false;
	IsGameStart = false;

	PostProcessVolume = GetWorld()->SpawnActor<APostProcessVolume>();
	PostProcessVolume->bUnbound = true;
	PostProcessVolume->SetActorLocation(FVector(1500.0f, 1134.0f, 2460.0f));
	PostProcessVolume->SetActorScale3D(FVector(86.0f, 44.0f, 20.0f));
	AddDynamicMaterialToPostProcessVolume();

	PC = Cast<ARolexPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		//UE_LOG(LogTemp, Error, TEXT("[AGM_TrainingRoom] PC : %s"), *PC->GetName());
		PC->InitUI();
		PC->SetCharacterOverlay();
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("[AGM_TrainingRoom] PC is nullptr"));
	}



	//if (HasAuthority())
	//{
	//	TArray<AActor*> foundActors;
	//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), foundActors);
	//
	//	for (AActor* actor : foundActors)
	//	{
	//		ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(actor);
	//		if (baseCharacter)
	//		{
	//			if (baseCharacter->Data.Team)
	//			{
	//				ATeamChracters.Add(baseCharacter);
	//				
	//			}
	//			else
	//			{
	//				BTeamChracters.Add(baseCharacter);
	//			}
	//		}
	//	}
	//}


	//GetWorld()->GetWorldSettings()->SetTimeDilation(0.1f);
	//
	//if (HasAuthority())
	//{
	//	// 맵에 있는 거점 가져오기
	//	AActor* foundPoint = UGameplayStatics::GetActorOfClass(GetWorld(), AActor_FightPoint::StaticClass());
	//	if (foundPoint)
	//	{
	//		AActor_FightPoint* point = Cast<AActor_FightPoint>(foundPoint);
	//		point->OnPointOverlapChanged.AddDynamic(this, &AGS_TrainingRoom::ChangeNumberOfTeam);
	//		Points.Add(point);
	//	}
	//
	//	// PlayTime 1초마다 1씩 올려주기
	//
	//}
}

void AGS_TrainingRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (not HasAuthority()) return;

	PlayTime += DeltaTime;
	if (PC)
	{
		PC->SetPlayTime(PlayTime);
		// 게임 시작 30초 후 거점 활성화
		if (PlayTime > 30.0f)
		{
			if (not IsActiveBsePoint) // 한번만 실행되도록 처리
			{
				AActor* foundPoint = UGameplayStatics::GetActorOfClass(GetWorld(), AActor_FightPoint::StaticClass());
				if (foundPoint)
				{
					AActor_FightPoint* point = Cast<AActor_FightPoint>(foundPoint);
					point->OnPointOverlapChanged.AddDynamic(this, &AGS_TrainingRoom::ChangeNumberOfTeam);
					Points.Add(point);
				}
				Points[0]->SetActivePoint(EActivePoint::Active);

				PC->SetPoint(0);

				IsActiveBsePoint = true;
			}
			// 아직 결과가 결정되지 않았을 경우
			if (Result == EResult::None)
			{
				// 거점 게이지 업데이트
				UpdatePointGauge(DeltaTime);

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
			else
			{
				// 게임이 끝났을 경우
				// 게임 결과를 클라이언트에게 전달
				if (not bIsGameOver)
				{
					
					PC->SetResult(Result);
					bIsGameOver = true;
				}
			}
		}
		// 게임 시작 15초 후 스폰지역 문 오픈
		else if (PlayTime > 15.0f)
		{
			if (not IsGameStart) // 한번만 실행되도록 처리
			{
				TArray<AActor*> foundActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor_StartDoor::StaticClass(), foundActors);
				for (AActor* found : foundActors)
				{
					AActor_StartDoor* door = Cast<AActor_StartDoor>(found);
					if (door)
					{
						door->SetGamePlayState(EGamePlayState::Start);
						Doors.Add(door);
					}
				}

				IsGameStart = true;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AGM_TrainingRoom] PC is nullptr"));
	}

}


void AGS_TrainingRoom::ChangeNumberOfTeam(bool bTeam, int32 ChangeValue)
{
	if (not HasAuthority()) return;

	UE_LOG(LogTemp, Warning, TEXT("[RolexGameMode ChangeNumberOfTeam] ChangeNumberOfTeam"));
	if (bTeam)
	{
		PointATeamCount += ChangeValue;
		UE_LOG(LogTemp, Warning, TEXT("[RolexGameMode ChangeNumberOfTeam] PointATeamCount: %d"), PointATeamCount);
	}
	else
	{
		PointBTeamCount += ChangeValue;
		UE_LOG(LogTemp, Warning, TEXT("[RolexGameMode ChangeNumberOfTeam] PointBTeamCount: %d"), PointBTeamCount);
	}
}
void AGS_TrainingRoom::UpdatePointGauge(float DeltaTime)
{
	// 거점을 아무도 점령하지 않은 상태
	if (Occupation == EOccupation::None)
	{

		// 거점에 A팀만 있는 상태
		if (PointATeamCount > 0 and PointBTeamCount == 0)
		{
			Clash = EClashing::None;

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
			Clash = EClashing::None;

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
			Clash = EClashing::None;

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
		else
		{
			Clash = EClashing::Clash;
		}
	}
	// 거점을 A팀이 점령하고 있는 상태
	else if (Occupation == EOccupation::TeamA)
	{
		if (PointATeamCount > 0  and PointBTeamCount > 0)
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
			PointATeamGaugePercent = 100.0f;
			Result = EResult::AWin;
			PC->SetOffofTxtraTime();
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
	// 거점을 B팀이 점령하고 있는 상태
	else if (Occupation == EOccupation::TeamB)
	{
		if (PointATeamCount > 0 and PointBTeamCount > 0)
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
			PointBTeamGaugePercent = 100.0f;
			Result = EResult::BWin;
			PC->SetOffofTxtraTime();
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



void AGS_TrainingRoom::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGS_TrainingRoom, PlayTime);
	DOREPLIFETIME(AGS_TrainingRoom, Points);
	DOREPLIFETIME(AGS_TrainingRoom, Doors);

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
	//if (HasAuthority())
	//{
	//	UE_LOG(LogTemp, Log, TEXT("[GS_OnRep_Server] PlayTime"));
	//	PC->SetPlayTime(PlayTime);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Log, TEXT("[GS_OnRep_Client] PlayTime"));
	//	PC->UI_Zone->SetPlayTime(PlayTime);
	//}
	PC->SetPlayTime(PlayTime);
}
void AGS_TrainingRoom::OnRep_Doors()
{
	for (AActor_StartDoor* door : Doors)
	{
		door->SetGamePlayState(EGamePlayState::Start);
	}
}
void AGS_TrainingRoom::OnRep_Points()
{
	//UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] Points"));
	int32 idx = 0;
	PC->SetPoint(idx);

	//for (AActor_FightPoint* point : Points)
	//{
	//	if (point->GetActivePoint() == EActivePoint::Active)
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("[GS OnRep_Points] Points : %s"), *point->GetName());
	//		UE_LOG(LogTemp, Log, TEXT("[GS OnRep_Points] idx : %d"), idx);
	//		PC->SetPoint(idx);
	//	}
	//	idx++;
	//}
}
void AGS_TrainingRoom::OnRep_Occupation()
{
	//UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] Occupation"));
}
void AGS_TrainingRoom::OnRep_Result()
{
	//UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] Result"));
	PC->SetOffofTxtraTime();
	PC->SetResult(Result);
}
void AGS_TrainingRoom::OnRep_Clash()
{
	//UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] Clash"));
	PC->SetClashing(Clash);
}
void AGS_TrainingRoom::OnRep_PointATeamCount()
{
	//UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] PointATeamCount"));
	//PC->UI_Zone->SetATeamCount(PointATeamCount);
	PC->SetATeamCount(PointATeamCount);
}
void AGS_TrainingRoom::OnRep_PointBTeamCount()
{
	//UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] PointBTeamCount"));
	//PC->UI_Zone->SetBTeamCount(PointBTeamCount);
	PC->SetBTeamCount(PointBTeamCount);
}
void AGS_TrainingRoom::OnRep_PointTakeGauge()
{
	//UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] PointTakeATeamGauge"));
	PC->SetTakingGuage(PointTakeATeamGauge, PointTakeBTeamGauge);
}
void AGS_TrainingRoom::OnRep_PointATeamGauge()
{
	//UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] PointATeamGauge"));
}
void AGS_TrainingRoom::OnRep_PointBTeamGauge()
{
	//UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] PointBTeamGauge"));
}
void AGS_TrainingRoom::OnRep_PointGaugePercent()
{
	//UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] PointATeamGaugePercent"));
	PC->SetPercent(Occupation ,PointATeamGaugePercent, PointBTeamGaugePercent);
}
void AGS_TrainingRoom::OnRep_WaitTime()
{
	//UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] WaitTime"));
}
void AGS_TrainingRoom::OnRep_ExtraTime()
{
	//UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] ExtraTime"));
	PC->SetExtraTime(ExtraTime);
}
void AGS_TrainingRoom::OnRep_ExtraTimeDecrease()
{
	//UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] ExtraTimeDecrease"));
}
void AGS_TrainingRoom::OnRep_IsGetATeamExtraTime()
{
	//UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] IsGetATeamExtraTime"));
	PC->SetIsATeamExtraTime(IsGetATeamExtraTime);
}
void AGS_TrainingRoom::OnRep_IsGetBTeamExtraTime()
{
	//UE_LOG(LogTemp, Log, TEXT("[GS_OnRep] IsGetBTeamExtraTime"));
	PC->SetIsBTeamExtraTime(IsGetBTeamExtraTime);
}

void AGS_TrainingRoom::AddDynamicMaterialToPostProcessVolume()
{
	if (PostProcessVolume && MaterialInstance)
	{
		//PostProcessVolume->AddOrUpdateBlendable(MaterialInstance);
	}
}