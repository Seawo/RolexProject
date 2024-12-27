// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Character_Muriel.generated.h"

/**
 * 
 */
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
	void MurielJump();								// 점프 입력
	void MurielRMBEnd();							// RMB 입력완료


	// 몽타주 실행함수
	void PlayMontage(FString Key, float InPlayRate = 1.0f, FName StartSectionName = NAME_None);

	void UpdateCoolTime(float DeltaTime);		// 쿨타임 업데이트 함수(Tick)

	public:
	void SpawnEffect(FName socketName, FName key);



	private:
	class UAnimInstance_Muriel* AnimInstance;			// 에니메이션 인스턴스
};
