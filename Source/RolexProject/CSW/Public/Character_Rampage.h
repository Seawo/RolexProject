// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Character_Rampage.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API ACharacter_Rampage : public ABaseCharacter
{
	GENERATED_BODY()
	ACharacter_Rampage();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ChangeAttackState(EAttackState state) override;
	//virtual void ChangeState(EMoveState state) override;
	virtual void InputAttack(const struct FInputActionValue& inputValue) override;
	
	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	

public:
	/*
	LBM, RBM, Q, E 
	*/
	/*UPROPERTY(ReplicatedUsing = OnRep_ChangeAttackState)*/
	EAttackState CurrAttackState;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEffectActor> StoneClass;
	
	class AEffectActor* Stone;


	UFUNCTION(Server, Reliable)
	void Server_ChangeAttackState(EAttackState attackState);
	UFUNCTION(NetMulticast, Reliable)
	void Multi_ChangeAttackState(EAttackState attackState);


	// Q 
	void CreateStone();
	void ThrowStone();


	UFUNCTION(Server, Reliable)
	void Server_CreateStone();


private:
	bool bIsRun = true;

	// LBM
	int32 MaxCnt;
	int32 ComboCnt;
	float ComboResetDelay;

	// E
	bool bIsDashing = false;
	float DashSpeed = 2000.0f;
	float DashDuration = 1.0f;
	FVector DashDirection;
	FVector StartPos;
	float ElapsedTime = 0.0f;
	float  DashHeight = 500.0f;
	
	// 콤보 리셋을 위한 타이머 핸들
	FTimerHandle ComboResetTimerHandle;
	FTimerHandle DashTimerHandle;

	// attackstate 
	void InputJump();
	void InputRun();
	void LBMAttack();
	void RBMAttack();
	void QAttack();
	void EAttack();
	void OutputNone(const struct FInputActionValue& inputValue);

	UFUNCTION(Server, Reliable)
	void Server_DashCheck(bool bIsDash);
	UFUNCTION(NetMulticast, Reliable)
	void Multi_DashCheck(bool bIsDash);



};
