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

	// LMB
	int32 MaxCnt = 5;
	int32 ComboCnt = 0;
	float MontageEndTime = 0.6f;
	FTimerHandle ComboResetTimerHandle;
	float ComboResetDelay = 1.5f;

	// RMB
	bool bIsShield = false;

	// E
	bool bIsDashing = false;
	float DashSpeed = 1500.0f; // 돌진 속도
	float DashTimeElapsed = 0.0f;
	float DashTimer = 4.0f;
	FVector DashDirection;

	UFUNCTION()
	void OnDashCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void StopEDash();
	
	// input
	void InputJump();
	void InputRun();
	void LMBAttack();
	void RMBAttack();
	void QAttack();
	void EAttack();
	
	// completed
	void RMBCompleted();
	void QCompleted();
};
