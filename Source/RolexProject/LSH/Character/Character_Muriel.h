// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Character_Muriel.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EQkillMovement : uint8
{
	Ascending,				// 수직 상승
	MovingHorizontally,	// 수평 이동
	Descending,			// 수직 하강
	Idle					// 대기 상태
};


UCLASS()
class ROLEXPROJECT_API ACharacter_Muriel : public ABaseCharacter
{
	GENERATED_BODY()
	
	ACharacter_Muriel();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ChangeAttackState(EAttackState state) override;
	virtual void InputAttack(const struct FInputActionValue& inputValue) override;
	void ChangeState(EMoveState state);					// 상태 변경

	UFUNCTION(Server, Reliable)
	void Server_AttackState(EAttackState state);
	UFUNCTION(NetMulticast, Reliable)
	void Multi_AttackState(EAttackState state);




	/*Input Function*/
	void MurielLShift();								// 쉬프트 입력
	void MurielFly();									// 공중에 떠오르기
	void MurielFlyComplete();							// 공중에 떠오르기 완료
	void MurielJump();									// 점프 입력
	void MurielRMBEnd();								// RMB 입력완료
	void MurielQSkillComplete();						// Q스킬 입력완료
	void MurielESkillComplete();						// E스킬 입력완료
	void MurielESkillRotate();							// E스킬 회전

	// 몽타주 실행함수
	void PlayAttackMontage(FString Key, float InPlayRate = 1.0f, FName StartSectionName = NAME_None);

	UFUNCTION(Server, Reliable)
	void Server_MoveModeChange(EMovementMode moveMode);
	UFUNCTION(Server, Reliable)
	void Server_MoveToLocation(FVector newLocation);

	void UpdateQSkillMovement(float DeltaTime);			// Q스킬 이동 업데이트 함수(Tick)
	void UpdateQSkillSearchPlayer();					// Q스킬 플레이어 찾기



	UFUNCTION(Server, Reliable)
	void Server_UpdateQSkillMovement(EQkillMovement state);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_UpdateQSkillMovement(EQkillMovement state, float time);

public:
	UFUNCTION(Server, Reliable)
	void Server_SpawnEffect(FName socketName, FName key);

	void SpawnEffect(FName socketName, FName key);



	bool GetIsRMBCharging() { return bIsRMBCharging; }	// RMB 충전중인지 여부 반환
	bool GetIsPlayingQSkill() { return bStartQSkill; }	// Q스킬 찾는중인지 여부 반환
	float GetSlopeForwrdAngle() const { return SlopeForwardAngle; }	// 경사로 각도 반환



/**변수들*/
private:
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_ERotate;


	class UAnimInstance_Muriel* AnimInstance;				// 에니메이션 인스턴스
	class ABaseCharacter* NearTeamCharacter;				// 가까운 팀 캐릭터

	float SlopeForwardAngle = 0.0f;
	float DefaultGravityScale = 1.0f;						// 기본 중력 스케일
	float ESkillRotationYaw = 0.0f;							// E스킬 Yaw값
	float SpacebarHoldTime = 0.0f;							// 스페이스바 누르고 있는 시간
	const float FlyThreshold = 0.5f;						// Fly로 전환되는 기준 시간 (0.5초)
	float FlyGauge = 5.0f;									// 공중에 떠있는 게이지


	bool bIsRMBCharging = false;							// RMB 충전중인지 여부
	bool bIsSearchQSkill = false;							// Q스킬 찾는중인지
	bool bIsESkillCharge = false;							// Q스킬 차징중인지
	bool bIsESkillRotate = false;							// E스킬 회전중인지

	bool bLShift = true;									// 쉬프트를 눌렀는지
	bool bIsPushSpaceBar = false;							// 스페이스바를 눌렀는지 ( 눌렀으면 공중에 떠오르기)
	bool bStartQSkill = false;								// Q스킬 시작했는지
	bool bStartESkill = false;								// E스킬 시작했는지

	FVector ESkillSpawnLocation;							// E스킬 생성 위치
	FRotator ESkillSpawnRotation;							// E스킬 생성 회전값

	// QSkill 이동할 위치값
	EQkillMovement QSkillMovement = EQkillMovement::Idle;	// Q스킬 이동 상태
	FVector QSkillStartLocation;							// 시작 지점
	FVector QSkillTargetLocation;							// 도착 지점
	float QSkillHorizontalSpeed = 10000.f;					// 수평 이동 속도
	float QSkillVerticalSpeed = 10000.f;					// 수직 이동 속도
	float ZHeight = 10000.0f;								// 수직 이동 높이



public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TMap<FName, TSubclassOf<class AActor_Effect>> EffectMap;

	float RMBRefillTiem = 5.0f;			// 쿨타임 5초
	float FlyRefillTiem = 10.0f;			// 쿨타임 5초
	float ESkillRefillTiem = 20.0f;		// 쿨타임 20초
	float QSkillRefillTiem = 60.0f;		// 쿨타임 1분
};
