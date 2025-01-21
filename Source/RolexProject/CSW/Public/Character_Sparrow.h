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


	// arrow
	UPROPERTY(EditAnywhere)
	TMap<FName, TSubclassOf<class AEffectActor>> ArrowClass;

	// charge
	UPROPERTY(EditAnywhere)
	TMap<FName, TSubclassOf<class AEffectActor>> ChargingClass;

	// Q
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEffectActor> QEffectActorclass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	UMaterialInterface* DecalMaterial;

	class AEffectActor* EffectActor;
	class AEffectActor* QEffectActor;



	void AimOffsetLBM();
	void ShootingArrowLBM();
	void AimOffsetRBM();
	void ShootingArrowRBM();
	void AimOffsetQ();
	void ShootingArrowQ();

	UFUNCTION(Server, Reliable)
	void Server_SpawnQEffect(FVector pos);

	UFUNCTION(Server, Reliable)
	void Server_ChangeAttackState(EAttackState attackState);
	UFUNCTION(NetMulticast, Reliable)
	void Multi_ChangeAttackState(EAttackState attackState);

private:
	// 애니메이션 가져오기
	class USparrowAnimInstance* anim;

	// 데칼 
	class UDecalComponent* AimIndicator;

	// Jump
	bool bIsRun = false;

	// LBM
	bool bLBMIsCharging = false;
	
	// Q
	FVector SelectPos;
	bool bQIsCharging = false;

	// RBM
	bool bRBMIsCharging = false;
	
	// E
	FTimerHandle SkillETimer;
	int32 RepeatCount = 0;     // 반복 횟수 카운터
	int32 MaxRepeat = 4;       // 최대 반복 횟수

	// 조준점을 위한 핸들러
	FTimerHandle LBMAimTimerHandle;
	FTimerHandle QAimTimerHandle;
	FTimerHandle RBMAimTimerHandle;


	// attackstate 
	void InputJump();
	void InputRun();
	void CompleteRun();
	void LBMAttack();
	void RBMAttack();
	void QAttack();
	void EAttack();

	// spawnArrow
	UFUNCTION(Server, Reliable)
	void Server_SpawnArrow(FName arrowName);

	void SpawnArrow(FName arrowName);

	void SpawnCharge(FName chargeName);
};
