// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RampageAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API URampageAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SlopeForwardAngle = 0;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


protected:
	
};
