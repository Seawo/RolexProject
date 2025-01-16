// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GM_TrainingRoom.h"
#include "GS_TrainingRoom.generated.h"
/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API AGS_TrainingRoom : public AGameStateBase
{
	GENERATED_BODY()
	
	AGS_TrainingRoom();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void OnRep_PlayTime();
	UFUNCTION()
	void OnRep_Points();


	UFUNCTION()
	void OnRep_Occupation();
	UFUNCTION()
	void OnRep_Result();
	UFUNCTION()
	void OnRep_Clash();

	UFUNCTION()
	void OnRep_PointATeamCount();
	UFUNCTION()
	void OnRep_PointBTeamCount();

	UFUNCTION()
	void OnRep_PointTakeGauge();

	UFUNCTION()
	void OnRep_PointATeamGauge();
	UFUNCTION()
	void OnRep_PointBTeamGauge();
	UFUNCTION()
	void OnRep_PointGaugePercent();

	UFUNCTION()
	void OnRep_WaitTime();
	UFUNCTION()
	void OnRep_ExtraTime();
	UFUNCTION()
	void OnRep_ExtraTimeDecrease();

	UFUNCTION()
	void OnRep_IsGetATeamExtraTime();
	UFUNCTION()
	void OnRep_IsGetBTeamExtraTime();





public:
	UPROPERTY(VisibleAnywhere, Category = "Info")
	class APlayerController_TrainingRoom* PC;

	UPROPERTY(ReplicatedUsing = OnRep_PlayTime)
	float PlayTime = 0.0f;
	UPROPERTY(VisibleAnywhere, Category = "Info")
	bool IsActiveBsePoint = false;

	UPROPERTY(VisibleAnywhere, Category = "Info")
	TArray<class ABaseCharacter*> ATeamChracters;		// Team == true (A팀)
	UPROPERTY(VisibleAnywhere, Category = "Info")
	TArray<class ABaseCharacter*> BTeamChracters;		// Team == false (B팀)
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Points, Category = "Info")
	TArray<class AActor_FightPoint*> Points;			// 거점들



	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Occupation, Category = "Info")
	EOccupation Occupation = EOccupation::None;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Result, Category = "Info")
	EResult Result = EResult::None;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Clash, Category = "Info")
	EClashing Clash = EClashing::None;

/** 거점 관련 변수들*/
	// 각 팀에서 거점을 밟고 있는 인원 체크 변수
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_PointATeamCount, Category = "Info")
	int32 PointATeamCount = 0;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_PointBTeamCount, Category = "Info")
	int32 PointBTeamCount = 0;

	// 팀별 거점 점령하기 위한 게이지 (거점이 점령되지 않았을 경우나 상대팀 거점일 경우에 거점에 상대팀이 없을 때, 이 게이지가 올라가며, 이 게이지가 다 차면 본인팀 거점으로 점령)
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_PointTakeGauge, Category = "Info")
	float PointTakeATeamGauge = 0;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_PointTakeGauge, Category = "Info")
	float PointTakeBTeamGauge = 0;

	// 팀별 점령 게이지 (거점이 점령됬을 때, 점령한 팀의 게이지가 증가, 게이지가 100퍼되면 그 팀의 게임 승리)
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_PointATeamGauge, Category = "Info")
	float PointATeamGauge = 0;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_PointBTeamGauge, Category = "Info")
	float PointBTeamGauge = 0;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_PointGaugePercent, Category = "Info")
	float PointATeamGaugePercent = 0;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_PointGaugePercent, Category = "Info")
	float PointBTeamGaugePercent = 0;

	// 대기 시간 (거점 점령하기 위한 게이지가 일부 차있는 상태에서 그 팀이 거점을 밟고 있지 않을 경우에 게이지가 초기화되기 까지의 대기 시간)
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_WaitTime, Category = "Info")
	float WaitTime = 0;

	// 추가 시간 (거점 점령 퍼센트가 90퍼센트가 넘어 가게 될때 상대팀이 거점을 밟을 경우 추가 시간이 주어지며, 거점 게이지는 동결된다.)
	// (추가 시간은 시간이 지날수록 최대치가 줄어들며, 상대팀이 거점을 밟고 있을 경우 추가 시간은 줄어들지 않는다. 추가 시간이 0이 되면 거점 게이지는 다시 작동한다.)
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_ExtraTime, Category = "Info")
	float ExtraTime = 0;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_ExtraTimeDecrease, Category = "Info")
	float ExtraTimeDecrease = 0;

	// 추가 시간 여부 확인
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_IsGetATeamExtraTime, Category = "Info")
	bool IsGetATeamExtraTime = false;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_IsGetBTeamExtraTime, Category = "Info")
	bool IsGetBTeamExtraTime = false;
};
