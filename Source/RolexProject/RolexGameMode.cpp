// Copyright Epic Games, Inc. All Rights Reserved.

#include "RolexGameMode.h"

#include "BaseCharacter.h"
#include "RolexGameInstance.h"
#include "RolexProjectCharacter.h"
#include "RolexPlayerController.h"
#include "RolexPlayerState.h"
#include "GameState/GS_TrainingRoom.h"
#include "Kismet/GameplayStatics.h"
#include "Point/Actor_FightPoint.h"
#include "GameFramework/PlayerStart.h"

#include "UObject/ConstructorHelpers.h"

ARolexGameMode::ARolexGameMode()
{
	// set default pawn class to our Blueprinted character
	PrimaryActorTick.bCanEverTick = true;
	bUseSeamlessTravel = true;
	
	// // set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	//
	// if (PlayerPawnBPClass.Class != NULL)
	// {
	// 	DefaultPawnClass = PlayerPawnBPClass.Class;
	// }
}

void ARolexGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (not HasAuthority()) return;

	IsActiveBsePoint = false;

	// 맵 거점 정보 가져오기
	AActor* foundPoint = UGameplayStatics::GetActorOfClass(GetWorld(), AActor_FightPoint::StaticClass());
	if (foundPoint)
	{
		AActor_FightPoint* point = Cast<AActor_FightPoint>(foundPoint);
		point->OnPointOverlapChanged.AddDynamic(this, &ARolexGameMode::ChangeNumberOfTeam);
		Points.Add(point);
	}

	// PlayerController 가져오기
	//RolexPC = Cast<ARolexPlayerController>(GetWorld()->GetFirstPlayerController());
	//if (RolexPC)
	//{
	//	RolexPC->InitUI();
	//	UE_LOG(LogTemp, Warning, TEXT("[ARolexGameMode BeginPlay] RolexPC is valid"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("[ARolexGameMode BeginPlay] RolexPC is invalid"));
	//}

	
	GetWorld()->GetTimerManager().SetTimer(TimerHandleGS, [this]()
				{
					if (RolexPC and RolexGS)
					{
						GetWorld()->GetTimerManager().ClearTimer(TimerHandleGS);
					}

					if (RolexGS)
					{
						

						UE_LOG(LogTemp, Warning, TEXT("[ARolexGameMode BeginPlay] RolexGS is valid"));
						RolexGS->PlayTime = PlayTime;
						RolexGS->IsActiveBsePoint = false;

						//RolexGS->ATeamChracters = ATeamChracters;
						//RolexGS->BTeamChracters = BTeamChracters;
						//RolexGS->Points = Points;

						RolexGS->Occupation = EOccupation::None;
						RolexGS->Result = EResult::None;
						RolexGS->Clash = EClashing::None;

						RolexGS->PointATeamCount = 0;
						RolexGS->PointBTeamCount = 0;

						RolexGS->PointTakeATeamGauge = 0;
						RolexGS->PointTakeBTeamGauge = 0;

						RolexGS->PointATeamGauge = 0;
						RolexGS->PointBTeamGauge = 0;
						RolexGS->PointATeamGaugePercent = 0;
						RolexGS->PointBTeamGaugePercent = 0;

						RolexGS->WaitTime = 0;

						RolexGS->ExtraTime = 8.0f;
						RolexGS->ExtraTimeDecrease = 1.0f;

						RolexGS->IsGetATeamExtraTime = false;
						RolexGS->IsGetBTeamExtraTime = false;
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("[ARolexGameMode BeginPlay] RolexGS is invalid"));

						RolexGS = Cast<AGS_TrainingRoom>(GetWorld()->GetGameState());
					}

					if (RolexPC)
					{
						RolexPC->InitUI();
						UE_LOG(LogTemp, Warning, TEXT("[ARolexGameMode BeginPlay] RolexPC is valid"));
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("[ARolexGameMode BeginPlay] RolexPC is invalid"));
						RolexPC = Cast<ARolexPlayerController>(GetWorld()->GetFirstPlayerController());
					}

					

				}, 0.1f, true);

	// GameState 가져오고 초기화 시켜주기
	//RolexGS = Cast<AGS_TrainingRoom>(GetWorld()->GetGameState());
	//if (RolexGS)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("[ARolexGameMode BeginPlay] RolexGS is valid"));
	//	RolexGS->PlayTime = PlayTime;
	//	RolexGS->IsActiveBsePoint = false;
	//
	//	//RolexGS->ATeamChracters = ATeamChracters;
	//	//RolexGS->BTeamChracters = BTeamChracters;
	//	RolexGS->Points = Points;
	//
	//	RolexGS->Occupation = EOccupation::None;
	//	RolexGS->Result = EResult::None;
	//	RolexGS->Clash = EClashing::None;
	//
	//	RolexGS->PointATeamCount = 0;
	//	RolexGS->PointBTeamCount = 0;
	//
	//	RolexGS->PointTakeATeamGauge = 0;
	//	RolexGS->PointTakeBTeamGauge = 0;
	//
	//	RolexGS->PointATeamGauge = 0;
	//	RolexGS->PointBTeamGauge = 0;
	//	RolexGS->PointATeamGaugePercent = 0;
	//	RolexGS->PointBTeamGaugePercent = 0;
	//
	//	RolexGS->WaitTime = 0;
	//
	//	RolexGS->ExtraTime = 8.0f;
	//	RolexGS->ExtraTimeDecrease = 1.0f;
	//
	//	RolexGS->IsGetATeamExtraTime = false;
	//	RolexGS->IsGetBTeamExtraTime = false;
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("[ARolexGameMode BeginPlay] RolexGS is invalid"));
	//}
}

void ARolexGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (not HasAuthority()) return;

	PlayTime += DeltaTime;

	// 게임 시작하고 10초 전
	if (PlayTime < 10.0f)
	{
		if (RolexGS)
		{
			RolexGS->PlayTime = PlayTime;
		}
		if (RolexPC)
		{
			RolexPC->SetPlayTime(PlayTime);
		}
	}
	// 게임 시작하고 10초 후 (거점 활성화)
	else
	{
		// 거점 활성화 (한번만 실행되도록)
		if (IsActiveBsePoint == false)
		{
			//int32 random = FMath::RandRange(0, Points.Num() - 1);
			int32 random = 0;

 			Points[random]->SetActivePoint(EActivePoint::Active);
			
			if (RolexPC)
			{
				UE_LOG(LogTemp, Warning, TEXT("[ARolexGameMode Tick] RolexPC is valid"));
				RolexPC->SetPoint(random);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[ARolexGameMode Tick] RolexPC is nullptr"));
			}

			RolexGS->Points = Points;
			IsActiveBsePoint = true;
		}

		// 아직 결과가 결정되지 않았을 경우
		if (Result == EResult::None)
		{
			// 거점 게이지 업데이트 (추후에 변경될 수 있음)
			UpdatePointGauge(DeltaTime);

			// 거점 게이트 GameState에 업데이트
			if (RolexGS)
			{
				RolexGS->PlayTime = PlayTime;
				RolexGS->IsActiveBsePoint = IsActiveBsePoint;

				RolexGS->Occupation = Occupation;
				RolexGS->Result = Result;
				RolexGS->Clash = Clash;

				//RolexGS->ATeamChracters = ATeamChracters;
				//RolexGS->BTeamChracters = BTeamChracters;
				

				RolexGS->PointATeamCount = PointATeamCount;
				RolexGS->PointBTeamCount = PointBTeamCount;

				RolexGS->PointTakeATeamGauge = PointTakeATeamGauge;
				RolexGS->PointTakeBTeamGauge = PointTakeBTeamGauge;

				RolexGS->PointATeamGauge = PointATeamGauge;
				RolexGS->PointBTeamGauge = PointBTeamGauge;
				RolexGS->PointATeamGaugePercent = PointATeamGaugePercent;
				RolexGS->PointBTeamGaugePercent = PointBTeamGaugePercent;

				RolexGS->WaitTime = WaitTime;

				RolexGS->ExtraTime = ExtraTime;
				RolexGS->ExtraTimeDecrease = ExtraTimeDecrease;
				RolexGS->IsGetATeamExtraTime = IsGetATeamExtraTime;
				RolexGS->IsGetBTeamExtraTime = IsGetBTeamExtraTime;
			}

			// 서버의 UI값 업데이트
			if (RolexPC)
			{
				RolexPC->SetPlayTime(PlayTime);
				RolexPC->SetATeamCount(PointATeamCount);
				RolexPC->SetBTeamCount(PointBTeamCount);
				RolexPC->SetTakingGuage(PointTakeATeamGauge, PointTakeBTeamGauge);
				RolexPC->SetPercent(Occupation, PointATeamGaugePercent, PointBTeamGaugePercent);
				RolexPC->SetClashing(Clash);
				RolexPC->SetExtraTime(ExtraTime);

				RolexPC->SetIsATeamExtraTime(IsGetATeamExtraTime);
				RolexPC->SetIsBTeamExtraTime(IsGetBTeamExtraTime);

			}
		}

	}
}

UClass* ARolexGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	UE_LOG(LogTemp, Warning, TEXT("Main Level ------------------------------------------------------------------------ Choose Player ------------------------------------------------------------------------"));
	
	ARolexPlayerState* RolexPlayerState = InController->GetPlayerState<ARolexPlayerState>();
	URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetGameInstance());

	if (RolexPlayerState && RolexGameInstance)
	{
		//find selected hero class based on the UniqueID key
		RolexPlayerState->FindUniqueID();
		if (TSubclassOf<ABaseCharacter>* BaseCharacterFactory = RolexGameInstance->PlayerHeroSelections.Find(RolexPlayerState->UniqueID))
		{
			return *BaseCharacterFactory;	
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Failed to find BaseCharacter"));
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

//AActor* ARolexGameMode::ChoosePlayerStart_Implementation(AController* Player)
//{
//	// True == A팀, False == B팀
//	TArray<AActor*> foundPlayerStarts;
//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), foundPlayerStarts);
//	for (AActor* playerStart : foundPlayerStarts)
//	{
//		ABaseCharacter* player = Cast<ABaseCharacter>(Player->GetPawn());
//		ARolexPlayerState* RolexPlayerState = Player->GetPlayerState<ARolexPlayerState>();
//		if (RolexPlayerState and RolexPlayerState->Team) // A팀
//		{
//			if (playerStart->GetName().Contains(TEXT("PlayerStart_A")))
//			{
//				return playerStart;
//			}
//		}
//		else if (RolexPlayerState and RolexPlayerState->Team) // B팀
//		{
//			if (playerStart->GetName().Contains(TEXT("PlayerStart_B")))
//			{
//				return playerStart;
//			}
//		}
//		else
//		{
//			UE_LOG(LogTemp, Warning, TEXT("player is nullptr"));
//		}
//	}
//
//	//return nullptr;
//}

void ARolexGameMode::UpdatePointGauge(float DeltaTime)
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
			PointATeamGaugePercent = 100.0f;
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
	// 거점을 B팀이 점령하고 있는 상태
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
			PointBTeamGaugePercent = 100.0f;
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

void ARolexGameMode::ChangeNumberOfTeam(bool bTeam, int32 ChangeValue)
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

// // print PlayerHeroSelection TMap
// for (auto Pair : RolexGameInstance->PlayerHeroSelections)
// {
// 	if (FString Mapkey = Pair.Key; *Mapkey)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Key: %s"), *Mapkey);
// 		Message = FString::Printf(TEXT("Key: %s"), *Mapkey);
// 		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, Message);
// 	}
// 	if (TSubclassOf<ABaseCharacter> MapValue = Pair.Value)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *MapValue->GetName());
// 		Message = FString::Printf(TEXT("Name: %s"), *MapValue->GetName());
// 		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, Message);
// 	}
// }