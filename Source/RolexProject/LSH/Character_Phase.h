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
	
	ACharacter_Phase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ChangeAttackState(EAttackState state) override;
	virtual void InputAttack(const struct FInputActionValue& inputValue) override;

	void InputLShift();
	void ChangeState(EMoveState state);


	void PlayMontage(FString Key, float InPlayRate = 1.0f, FName StartSectionName = NAME_None);

public:
	void SpawnEffect(FName socketName, FName key);

protected:
	//void LMBActionBegin();
	//void LMBActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	//void SetLMBCheckTimer(class UAnimMontage* TargetMontage);
	//void LMBCheck(class UAnimMontage* TargetMontage);




protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UDataAsset_PhaseLMB> LMBData;
	int32 CurrentLMBCombo = 0;
	FTimerHandle LMBTimerHandle;
	bool bHasNextLMBCommand = false;

private:
	class UAnimInstance_Phase* AnimInstance;			// 에니메이션 인스턴스
	FVector AimDirection;								// 에임 방향

	bool bLShift = true;								// 쉬프트를 눌렀는지
	float LerpAlpha = 0.0f;
	float Duration = 5.0f;




public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TMap<FName, TSubclassOf<class AActor_Effect>> EffectMap;

	float SlopeForwardAngle = 0.0f;

	// 스킬 지속시간
	float LRSkillDuration = 3.0f;
	float QSkillDuration = 3.5f;
	float ESkillDuration = 8.0f;

	// 스킬 쿨타임
	float RMBSkillCoolTime = 0.0f;		// 쿨타임 5초
	float ESkillCoolTime = 0.0f;		// 쿨타임 20초
	float QSkillCoolTime = 0.0f;		// 쿨타임 1분
};
