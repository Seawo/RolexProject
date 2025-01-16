// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Actor_FightPoint.h"

#include "BaseCharacter.h"
#include "Net/UnrealNetwork.h"


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

	//if (HasAuthority())
	//{
	//	DrawDebugS(DeltaTime);
	//}
	//else
	//{
	//	DrawDebugS(DeltaTime);
	//}


	// 게임이 끝났거나, 거점이 비활성화라면 리턴해줘
	if (Finish != EFinish::None or ActivePoint == EActivePoint::Deactivate) return;
	DrawDebugS(DeltaTime);

	// 서버가 아니라면 계산 x
	//if (not HasAuthority()) return;

	//SetPointGauge(DeltaTime);
}

void AActor_FightPoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	//DOREPLIFETIME(AActor_FightPoint, MoveState);
}

void AActor_FightPoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Finish != EFinish::None or ActivePoint == EActivePoint::Deactivate) return;

	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UE_LOG(LogTemp, Warning, TEXT("[FightPoint] OnOverlapBegin"));

	ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);
	if (character and character->Data.Team == true)
	{
		ActiveATeamCount++;
		OnPointOverlapChanged.Broadcast(true, 1);
	}
	else if (character and character->Data.Team == false)
	{
		ActiveBTeamCount++;
		OnPointOverlapChanged.Broadcast(false, 1);
	}


	//SetTeam();
}

void AActor_FightPoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Finish != EFinish::None or ActivePoint == EActivePoint::Deactivate) return;



	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	UE_LOG(LogTemp, Warning, TEXT("[FightPoint] OnOverlapEnd"));

	ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);
	if (character and character->Data.Team == true)
	{
		ActiveATeamCount--;
		OnPointOverlapChanged.Broadcast(true, -1);
	}
	else if (character and character->Data.Team == false)
	{
		ActiveBTeamCount--;
		OnPointOverlapChanged.Broadcast(false, -1);
	}

	//SetTeam();
}

void AActor_FightPoint::DrawDebugS(float DeltaTime)
{
	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 0, 500), FString::Printf(TEXT("ActivePoint : %d"), ActivePoint), nullptr, FColor::Yellow, DeltaTime);

	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 0, 450), FString::Printf(TEXT("TakePointGauge : %.1f"), TakePointATeamGauge), nullptr, FColor::Red, DeltaTime);
	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 0, 400), FString::Printf(TEXT("TakePointGauge : %.1f"), TakePointBTeamGauge), nullptr, FColor::Blue, DeltaTime);
	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 0, 375), FString::Printf(TEXT("TakePointGauge : %.1f"), ExtraTimer), nullptr, FColor::Green, DeltaTime);
	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 0, 350), FString::Printf(TEXT("TakePointGauge : %.1f"), ExtraTimerDecrease), nullptr, FColor::Green, DeltaTime);




	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 0, 300), FString::Printf(TEXT("Team : %d"), Team), nullptr, FColor::Green, DeltaTime);


	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 0, 250), FString::Printf(TEXT("ATeamPointGauge : %.1f"), ATeamPointGauge / 120 * 100), nullptr, FColor::Red, DeltaTime);
	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 0, 225), FString::Printf(TEXT("ATeamPointGauge : %d"), IsAddATeamExtraTime), nullptr, FColor::Red, DeltaTime);
	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 0, 200), FString::Printf(TEXT("ActiveATeamCount : %d"), ActiveATeamCount), nullptr, FColor::Red, DeltaTime);

	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 0, 100), FString::Printf(TEXT("BTeamPointGauge : %.1f"), BTeamPointGauge / 120 * 100), nullptr, FColor::Blue, DeltaTime);
	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 0, 75), FString::Printf(TEXT("BTeamPointGauge : %d"), IsAddBTeamExtraTime), nullptr, FColor::Blue, DeltaTime);
	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 0, 50), FString::Printf(TEXT("ActiveBTeamCount : %d"), ActiveBTeamCount), nullptr, FColor::Blue, DeltaTime);
}

void AActor_FightPoint::SetPointGauge(float DeltaTime)
{
	{
		Multi_SetPointGauge(DeltaTime);
	}
}

void AActor_FightPoint::Multi_SetPointGauge_Implementation(float DeltaTime)
{
	if (Team == ETeam::None)
	{

		// 거점에 A팀만 있는 상태
		if (ActiveATeamCount > 0 and ActiveBTeamCount == 0)
		{
			Timer = 0;
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
			Timer = 0;
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
		else if (ActiveATeamCount == 0 and ActiveBTeamCount == 0)
		{
			Timer += DeltaTime;
			if (Timer >= 3.0f)
			{
				TakePointATeamGauge -= 50 * DeltaTime;
				TakePointBTeamGauge -= 50 * DeltaTime;

				if (TakePointATeamGauge <= 0)
				{
					TakePointATeamGauge = 0.0f;
				}

				if (TakePointBTeamGauge <= 0)
				{
					TakePointBTeamGauge = 0.0f;
				}

				if (TakePointATeamGauge == 0 and TakePointBTeamGauge == 0)
				{
					Timer = 0.0f;
				}
			}
		}
	}
	else if (Team == ETeam::TeamA)
	{
		// 게이지가 다 차게되면 A팀 승리
		if (ATeamPointGauge >= 120.0f)
		{
			// A팀 승리
			UE_LOG(LogTemp, Warning, TEXT("ATeam Win"));
			Finish = EFinish::AWin;
			return;
		}

		// 게이지값을 퍼센트로 변경
		ATeamGaugePercent = ATeamPointGauge / 120 * 100;

		// A팀 게이지가 90% 이상이고 B팀이 거점 안에 한번이라도 들어왔을 경우
		if (ATeamGaugePercent >= 90.0f and ActiveBTeamCount > 0)
		{
			IsAddBTeamExtraTime = true;
		}

		// 추가 시간이 있을 경우
		if (IsAddBTeamExtraTime)
		{
			// A팀 게이지가 99.5% 이상이면
			if (ATeamGaugePercent >= 99.5f)
			{
				// 추가시간이 끝났다면
				if (ExtraTimer <= 0)
				{
					// A팀 게이지 다시 감소
					ATeamPointGauge += DeltaTime;
				}
				// 추가시간이 끝나지 않았다면
				else
				{
					// A팀 게이지를 99.5%로 고정
					ATeamGaugePercent = 99.5f;

					// B팀이 거점에 들어왔을 경우
					if (ActiveBTeamCount > 0)
					{
						ExtraTimer = 8.0f;

						// 추가시간의 감소치를 증가시킨다.
						if (ExtraTimerDecrease > 4.0f)
						{
							ExtraTimerDecrease = 4.0f;
						}
						else
						{
							ExtraTimerDecrease += DeltaTime;
						}
					}
					else
					{
						// 추가 시간 차감
						ExtraTimer -= ExtraTimerDecrease * DeltaTime;
					}
				}
			}
			// A팀의 게이지가 99.5%보다 낮다면 계속 게이지 증가
			else
			{
				ATeamPointGauge += DeltaTime;
			}
		}
		// 추가 시간이 없을 경우
		else
		{
			ATeamPointGauge += DeltaTime;
		}

		// A팀은 없고 B팀만 거점에 있을 경우
		// ->거점을 점령하기 위한게이지가 증가, 게이지가 100이 되면 B팀 거점으로 변경
		if (ActiveATeamCount == 0 and ActiveBTeamCount > 0)
		{
			TakePointBTeamGauge += 15 * ActiveBTeamCount * DeltaTime;

			if (TakePointBTeamGauge >= 100.0f)
			{
				TakePointBTeamGauge = 0.0f;
				IsAddBTeamExtraTime = false;
				Team = ETeam::TeamB;
			}
		}

		// A팀과는 무관하게 B팀이 거점에 없을 경우
		// -> 거점 점령 대기시간이 줄어들면서 점령하기 위한 게이지가 줄어든다
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
			Finish = EFinish::BWin;
			return;
		}

		BTeamGaugePercent = BTeamPointGauge / 120 * 100;
		// B팀 게이지가 90% 이상이고 A팀이 거점 안에 한번이라도 들어왔을 경우
		if (BTeamGaugePercent >= 90.0f and ActiveATeamCount > 0)
		{
			IsAddATeamExtraTime = true;
		}

		if (IsAddATeamExtraTime)
		{
			// B팀 게이지가 99.5% 이상이면
			if (BTeamGaugePercent >= 99.5f)
			{
				// 추가시간이 끝났다면
				if (ExtraTimer <= 0)
				{
					// B팀 게이지 다시 감소
					BTeamPointGauge += DeltaTime;
				}
				// 추가시간이 끝나지 않았다면
				else
				{
					// B팀 게이지를 99.5%로 고정
					BTeamGaugePercent = 99.5f;
					// A팀이 거점에 들어왔을 경우
					if (ActiveATeamCount > 0)
					{
						ExtraTimer = 8.0f;

						// 추가시간의 감소치를 증가시킨다.
						if (ExtraTimerDecrease > 4.0f)
						{
							ExtraTimerDecrease = 4.0f;
						}
						else
						{
							ExtraTimerDecrease += DeltaTime;
						}
					}
					else
					{
						// 추가 시간 차감
						ExtraTimer -= ExtraTimerDecrease * DeltaTime;
					}
				}
			}
			// B팀의 게이지가 99.5%보다 낮다면 계속 게이지 증가
			else
			{
				BTeamPointGauge += DeltaTime;
			}
		}
		else
		{
			BTeamPointGauge += DeltaTime;
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
}

void AActor_FightPoint::OnRep_Team()
{
}

