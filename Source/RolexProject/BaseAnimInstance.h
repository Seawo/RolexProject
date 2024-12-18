// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseCharacter.h"
#include "BaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

	virtual void NativeInitializeAnimation() override;


	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
	EMoveState MoveState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
	EAttackState AttackState;
};
