// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Character_Steel.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API ACharacter_Steel : public ABaseCharacter
{
	GENERATED_BODY()

	ACharacter_Steel();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ChangeAttackState(EAttackState state) override;
	//virtual void ChangeState(EMoveState state) override;
	virtual void InputAttack(const struct FInputActionValue& inputValue) override;

public:
	EAttackState CurrAttackState;





private:
	bool bIsRun = false;

	// LBM
	int32 MaxCnt = 5;
	int32 ComboCnt = 0;
	float MontageEndTime = 0.6f;
	FTimerHandle ComboResetTimerHandle;
	float ComboResetDelay = 1.5f;

	// input
	void InputJump();
	void InputRun();
	void LMBAttack();
	void RMBAttack();
	void QAttack();
	void EAttack();

};
