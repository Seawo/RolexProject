// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Animation/AnimMontage.h"
#include "Character_Phase.generated.h"

/**
 * 
 */

UCLASS()
class ROLEXPROJECT_API ACharacter_Phase : public ABaseCharacter
{
	GENERATED_BODY()
	
	// 생성자 함수
	ACharacter_Phase();

	// 기본 함수
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 상태 변화 함수들
	virtual void ChangeAttackState(EAttackState state) override;


	// Input 함수들
	virtual void InputAttack(const struct FInputActionValue& inputValue) override;
	void PhaseJump();
	void InputLShift();							// 쉬프트 입력 


	// 공격 함수들
	void LBMAttack();
	void RBMAttack();
	void QAttack();
	void EAttack();




	// 몽타주 실행 및 이펙트 생성 함수
	void PlayMontage(FString Key, float InPlayRate = 1.0f, FName StartSectionName = NAME_None);
public:
	void SpawnEffect(FName socketName, FName key);

private:
	// Tick에서 계속 돌아가야하는 함수들
	void UpdateUI();							// UI 업데이트 함수(Tick)
	//void UpdateCoolTime(float DeltaTime);		// 쿨타임 업데이트 함수(Tick)




/**********변수들**********/
private:
	class UAnimInstance_Phase* AnimInstance;			// 에니메이션 인스턴스
	FVector AimDirection;								// 에임 방향

	bool bLShift = true;								// 쉬프트를 눌렀는지
	bool bIsJumping = false;							// 점프중인지
	




	float LerpAlpha = 0.0f;
	float Duration = 5.0f;


	UPROPERTY(EditAnywhere, Category = "Effect")
	class UAudioComponent_Phase* AudioComponent;		// 오디오 컴포넌트

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUI_InGame> UI_InGameClass;		// UI 클래스

	UPROPERTY()
	class UUI_InGame* UI_InGame;						// UI 클래스 인스턴스

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TMap<FName, TSubclassOf<class AActor_Effect>> EffectMap;	// 이펙트 맵


	float SlopeForwardAngle = 0.0f;						// 경사로 앞으로 가는 각도 (달릴때의 몸 기울기 각도값)
	bool bIsPlayingQSkill = false;						// Q스킬이 실행중인지


	// 스킬 지속시간
	float LRSkillDuration = 3.0f;						// 좌클릭, 우클릭 스킬 생성시간
	float QSkillDuration = 3.5f;						// Q스킬 생성시간	
	float ESkillDuration = 8.0f;						// E스킬 생성시간

	// 스킬 쿨타임
	float RMBSkillCoolTime = 0.0f;						// 쿨타임 5초
	float ESkillCoolTime = 0.0f;						// 쿨타임 20초
	float QSkillCoolTime = 0.0f;						// 쿨타임 1분

	float RMBRefillTime = 5.0f;							// 쿨타임 5초
	float ESkillRefillTime = 20.0f;						// 쿨타임 20초
	float QSkillRefillTime = 60.0f;						// 쿨타임 1분
};
