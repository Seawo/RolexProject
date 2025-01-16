// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RolexGameMode.h"
#include "GM_TrainingRoom.generated.h"

// 거점 점령 상태를 알려주는 Enum값
UENUM(BlueprintType)
enum class EOccupation : uint8
{
	None,				// 점령 안된 상태
	TeamA,				// A팀점령
	TeamB,				// B팀점령
};


// 거점 결과를 알려주는 Enum값
UENUM(BlueprintType)
enum class EResult : uint8
{
	None,
	AWin,
	BWin,
	draw,
};

UENUM(BlueprintType)
enum class EClashing : uint8
{
	None,				// 충돌 없는 상태	
	Clash,				// A팀과 B팀이 충돌하는 상태
};

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API AGM_TrainingRoom : public ARolexGameMode
{
	GENERATED_BODY()
	
	AGM_TrainingRoom();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, class AActor* StartSpot) override;



	void UpdatePointGauge(float DeltaTime);

	// 각 팀의 인원 변경값 함수
	UFUNCTION()
	void ChangeNumberOfTeam(bool bTeam, int32 ChangeValue);

public:
	void SetPlayTime(float time) { PlayTime += time; }
	float GetPlayTime() { return PlayTime; }

private:
	class AGS_TrainingRoom* GS;
	class APlayerController_TrainingRoom* PC;

	bool IsActiveBsePoint = false;

	float PlayTime = 0.0f;
	float BasePoint = 0.0f;

public:


	UPROPERTY(VisibleAnywhere, Category = "Info")
	TArray<class ABaseCharacter*> ATeamChracters;		// Team == true

	UPROPERTY(VisibleAnywhere, Category = "Info")
	TArray<class ABaseCharacter*> BTeamChracters;		// Team == false

	UPROPERTY(VisibleAnywhere, Category = "Info")
	TArray<class AActor_FightPoint*> Points;




	EOccupation Occupation = EOccupation::None;
	EResult Result = EResult::None;
	EClashing Clash = EClashing::None;

	/** 거점 관련 변수들*/
		// 각 팀에서 거점을 밟고 있는 인원 체크 변수
	int32 PointATeamCount = 0;
	int32 PointBTeamCount = 0;

	// 팀별 거점 점령하기 위한 게이지 (거점이 점령되지 않았을 경우나 상대팀 거점일 경우에 거점에 상대팀이 없을 때, 이 게이지가 올라가며, 이 게이지가 다 차면 본인팀 거점으로 점령)
	float PointTakeATeamGauge = 0;
	float PointTakeBTeamGauge = 0;

	// 팀별 점령 게이지 (거점이 점령됬을 때, 점령한 팀의 게이지가 증가, 게이지가 100퍼되면 그 팀의 게임 승리)
	float PointATeamGauge = 0;
	float PointBTeamGauge = 0;
	float PointATeamGaugePercent = 0;
	float PointBTeamGaugePercent = 0;

	// 대기 시간 (거점 점령하기 위한 게이지가 일부 차있는 상태에서 그 팀이 거점을 밟고 있지 않을 경우에 게이지가 초기화되기 까지의 대기 시간)
	float WaitTime = 0;

	// 추가 시간 (거점 점령 퍼센트가 90퍼센트가 넘어 가게 될때 상대팀이 거점을 밟을 경우 추가 시간이 주어지며, 거점 게이지는 동결된다.)
	// (추가 시간은 시간이 지날수록 최대치가 줄어들며, 상대팀이 거점을 밟고 있을 경우 추가 시간은 줄어들지 않는다. 추가 시간이 0이 되면 거점 게이지는 다시 작동한다.)
	float ExtraTime = 8.0f;
	float MaxExtraTime = 8.0f;
	float ExtraTimeDecrease = 1.0f;

	// 추가 시간 여부 확인
	bool IsGetATeamExtraTime = false;
	bool IsGetBTeamExtraTime = false;
};
