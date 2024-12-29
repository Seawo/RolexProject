// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "AnimInstance_Muriel.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API UAnimInstance_Muriel : public UBaseAnimInstance
{
	GENERATED_BODY()
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;



	UFUNCTION()
	void AnimNotify_RMBCharge();

public:
	class ACharacter_Muriel* Muriel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
	float SlopeForwardAngle;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData")
	//bool bIsPlayingQSkill = false;
};
