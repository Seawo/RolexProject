// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor_Point.h"
#include "GameFramework/Actor.h"
#include "Actor_FightPoint.generated.h"

// 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPointOverlapDelegate, bool, bTeam, int32, ChangeValue);

// 거점 활성화 비활성화를 위한 Enum값
UENUM(BlueprintType)
enum class EActivePoint : uint8
{
	Deactivate,
	Active,
};

// 거점 상태를 알려주는 Enum값
UENUM(BlueprintType)
enum class ETeam : uint8
{
	None,				// 점령 안된 상태
	TeamA,				// A팀점령
	TeamB,				// B팀점령
	Clashing,			// A팀과 B팀이 충돌하는 상태
};

UENUM(BlueprintType)
enum class EFinish : uint8
{
	None,
	AWin,
	BWin,
	draw,
};

UCLASS()
class ROLEXPROJECT_API AActor_FightPoint : public AActor_Point
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_FightPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	//UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	void DrawDebugS(float DeltaTime);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_SetPointGauge(float DeltaTime);


	void SetActivePoint(EActivePoint activePoint) { ActivePoint = activePoint; }
	EActivePoint GetActivePoint() const { return ActivePoint; }
	ETeam GetTeam() const { return Team; }
	
public:
	UPROPERTY()
	FPointOverlapDelegate OnPointOverlapChanged;




private:
	UPROPERTY(VisibleAnywhere)
	int32 ActiveATeamCount = 0;
	UPROPERTY(VisibleAnywhere)
	int32 ActiveBTeamCount = 0;

	float ATeamPointGauge = 0.0f;				// 최대치 120
	float ATeamGaugePercent = 0.0f;				// 최대치 100%
	float BTeamPointGauge = 0.0f;				// 최대치 120	
	float BTeamGaugePercent = 0.0f;				// 최대치 100%

	float TakePointATeamGauge = 0.0f;			// 최대치 100
	float TakePointBTeamGauge = 0.0f;			// 최대치 100

	float Timer = 0.0f;							// 대기 타이머
	float ExtraTimer = 8.0f;					// 추가 타이머
	float ExtraTimerDecrease = 1.0f;			// 추가 타이머 감소량

	bool IsAddATeamExtraTime = false;			// A팀 추가 시간여부 (B팀 거점일 경우)
	bool IsAddBTeamExtraTime = false;			// B팀 추가 시간여부 (A팀 거점일 경우)

	EActivePoint ActivePoint = EActivePoint::Deactivate;
	ETeam Team = ETeam::None;

	EFinish Finish = EFinish::None;
};
