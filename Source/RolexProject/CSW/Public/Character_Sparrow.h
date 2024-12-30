// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Character_Sparrow.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API ACharacter_Sparrow : public ABaseCharacter
{
	GENERATED_BODY()

	ACharacter_Sparrow();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ChangeAttackState(EAttackState state) override;
	//virtual void ChangeState(EMoveState state) override;
	virtual void InputAttack(const struct FInputActionValue& inputValue) override;

public:
	/*
	LBM, RBM, Q, E
	*/
	EAttackState CurrAttackState;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEffectActor> ArrowClass;

	class AEffectActor* Arrow;

	void AimOffsetLBM();
	void ShootingArrowLBM();
	void AimOffsetQ();
	void ShootingArrowQ();

private:
	// 애니메이션 가져오기
	class USparrowAnimInstance* anim;

	// Jump
	bool bIsRun = false;

	// LBM
	bool bIsCharging = false;

	// E

	// 조준점을 위한 핸들러
	FTimerHandle LBMAimTimerHandle;
	FTimerHandle QAimTimerHandle;

	// attackstate 
	void InputJump();
	void InputRun();
	void CompleteRun();
	void LBMAttack();
	void RBMAttack();
	void QAttack();
	void EAttack();

};
