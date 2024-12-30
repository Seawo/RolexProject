// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "SparrowAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API USparrowAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlopeForwardAngle = 0;

	// Pitch와 Yaw 변수 선언
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Pitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator CameraRot;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void UpdatePitchYaw(float NewPitch);
};
