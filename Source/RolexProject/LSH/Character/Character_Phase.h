// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Animation/AnimMontage.h"
#include "Character_Phase.generated.h"

/**
 * 
 */
 USTRUCT(BlueprintType)
 struct FEffectData
 {
	 GENERATED_BODY()

	 FName EffectName;
	 TSubclassOf<class AActor_Effect> EffectClass;
 };


UCLASS()
class ROLEXPROJECT_API ACharacter_Phase : public ABaseCharacter
{
	GENERATED_BODY()
	
	ACharacter_Phase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ChangeAttackState(EAttackState state) override;
	virtual void InputAttack(const struct FInputActionValue& inputValue) override;
	void PhaseJump();
	void PhaseJumpEnd();


	void InputLShift();							// 쉬프트 입력 
	void ChangeState(EMoveState state);			// 상태 변경


	// 몽타주 실행함수
	void PlayMontage(FString Key, float InPlayRate = 1.0f, FName StartSectionName = NAME_None);
	void UpdateUI();							// UI 업데이트 함수(Tick)
	void UpdateCoolTime(float DeltaTime);		// 쿨타임 업데이트 함수(Tick)

public:
	void SpawnEffect(FName socketName, FName key);

protected:

protected:



private:
	class UAnimInstance_Phase* AnimInstance;			// 에니메이션 인스턴스
	FVector AimDirection;								// 에임 방향

	bool bLShift = true;								// 쉬프트를 눌렀는지
	bool bIsJumping = false;							// 점프중인지
	




	float LerpAlpha = 0.0f;
	float Duration = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Effect")
	class UAudioComponent_Phase* AudioComponent;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUI_InGame> UI_InGameClass;

	UPROPERTY()
	class UUI_InGame* UI_InGame;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TMap<FName, TSubclassOf<class AActor_Effect>> EffectMap;


	float SlopeForwardAngle = 0.0f;
	bool bIsPlayingQSkill = false;


	// 스킬 지속시간
	float LRSkillDuration = 3.0f;
	float QSkillDuration = 3.5f;
	float ESkillDuration = 8.0f;

	// 스킬 쿨타임
	float RMBSkillCoolTime = 0.0f;		// 쿨타임 5초
	float ESkillCoolTime = 0.0f;		// 쿨타임 20초
	float QSkillCoolTime = 0.0f;		// 쿨타임 1분
};
