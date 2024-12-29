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
	void ChangeState(EMoveState state);			// 상태 변경


	/*Input Function*/
	void MurielLShift();							// 쉬프트 입력
	void MurielFly();								// 공중에 떠오르기
	void MurielJump();								// 점프 입력
	void MurielRMBEnd();							// RMB 입력완료
	void MurielQSkillComplete();					// Q스킬 입력완료


	// 몽타주 실행함수
	void PlayAttackMontage(FString Key, float InPlayRate = 1.0f, FName StartSectionName = NAME_None);
	void PlayStateMontage(FString Key, float InPlayRate = 1.0f, FName StartSectionName = NAME_None);

	void UpdateCoolTime(float DeltaTime);		// 쿨타임 업데이트 함수(Tick)

	void UpdateQSkillMovement(float DeltaTime);	// Q스킬 이동 업데이트 함수(Tick)

public:
	void SpawnEffect(FName socketName, FName key);




	bool GetIsRMBCharging() { return bIsRMBCharging; }	// RMB 충전중인지 여부 반환
	float GetSlopeForwrdAngle() const { return SlopeForwardAngle; }	// 경사로 각도 반환



/**변수들*/
private:
	class UAnimInstance_Muriel* AnimInstance;			// 에니메이션 인스턴스

	float SlopeForwardAngle = 0.0f;
	float FlyingTime = 7.0f;							// 공중에 떠있는 시간 (5초)
	float DefaultGravityScale = 1.0f;					// 기본 중력 스케일

	bool bIsRMBCharging = false;						// RMB 충전중인지 여부
	bool bLShift = true;								// 쉬프트를 눌렀는지
	bool bIsPushSpaceBar = false;						// 스페이스바를 눌렀는지 ( 눌렀으면 공중에 떠오르기)
	bool bStartQSkill = false;							// Q스킬 시작했는지

	// QSkill 이동할 위치값
	EQkillMovement QSkillMovement = EQkillMovement::Idle;	// Q스킬 이동 상태
	FVector QSkillStartLocation;							// 시작 지점
	FVector QSkillTargetLocation;							// 도착 지점
	float QSkillHorizontalSpeed = 1;						// 수평 이동 속도
	float QSkillVerticalSpeed = 1;							// 수직 이동 속도
	float ZHeight = 1000.0f;								// 수직 이동 높이



	// 쿨타임
	float RMBSkillCoolTime = 0.0f;		// 쿨타임 5초
	float FlyCoolTime = 0.0f;		// 쿨타임 5초
	float ESkillCoolTime = 0.0f;		// 쿨타임 20초
	float QSkillCoolTime = 0.0f;		// 쿨타임 1분
};
