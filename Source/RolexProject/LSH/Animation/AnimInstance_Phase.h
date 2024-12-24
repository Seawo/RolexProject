// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "AnimInstance_Phase.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API UAnimInstance_Phase : public UBaseAnimInstance
{
	GENERATED_BODY()
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


	// 노티파이 함수
	UFUNCTION()
	void AnimNotify_Attack1Spawn();
	UFUNCTION()
	void AnimNotify_Attack2Spawn();
	UFUNCTION()
	void AnimNotify_Attack3Spawn();
	UFUNCTION()
	void AnimNotify_Attack4Spawn();
	UFUNCTION()
	void AnimNotify_LMBAttack();
	UFUNCTION()
	void AnimNotify_QAttack();
	UFUNCTION()
	void AnimNotify_EAttack();

	public:
	class ACharacter_Phase* Phase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
	float SlopeForwardAngle;
};
