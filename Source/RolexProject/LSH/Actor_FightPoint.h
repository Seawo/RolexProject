// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor_Point.h"
#include "GameFramework/Actor.h"
#include "Actor_FightPoint.generated.h"

UENUM(BlueprintType)
enum class EActivePoint : uint8
{
	Deactivate,
	Active,
};

UENUM(BlueprintType)
enum class ETeam : uint8
{
	None,				// 아무도 없는 상태
	TeamA,				// A팀만 있는 상태
	TeamB,				// B팀만 있는 상태
	Clashing,			// A팀과 B팀이 충돌하는 상태
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


	//UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	//UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;


	void UpdateTakePointGauge(float DeltaTime);
	void UpdateTeamPointGauge(float DeltaTime);

	void SetActivePoint(EActivePoint activePoint) { ActivePoint = activePoint; }
	ETeam GetTeam() const { return Team; }
	void SetPointGauge(float DeltaTime);

private:
	UPROPERTY(VisibleAnywhere)
	int32 ActiveATeamCount = 0;
	UPROPERTY(VisibleAnywhere)
	int32 ActiveBTeamCount = 0;

	float ATeamPointGauge = 0.0f;				// 최대치 100
	float BTeamPointGauge = 0.0f;				// 최대치 100	

	float TakePointATeamGauge = 0.0f;			// 최대치 100
	float TakePointBTeamGauge = 0.0f;			// 최대치 100

	float Timer = 0.0f;							// 대기 타이머

	EActivePoint ActivePoint = EActivePoint::Deactivate;
	ETeam Team = ETeam::None;
};
