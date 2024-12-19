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

	public:
	class ACharacter_Phase* Phase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
	float SlopeForwardAngle;
};
