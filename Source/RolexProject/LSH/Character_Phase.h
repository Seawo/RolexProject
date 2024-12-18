// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Character_Phase.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API ACharacter_Phase : public ABaseCharacter
{
	GENERATED_BODY()
	
	ACharacter_Phase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ChangeState(EAttackState state) override;
	virtual void InputAttack(const struct FInputActionValue& inputValue) override;

	void InputLShift();


	void PlayMontage(FString Key, float InPlayRate = 1.0f, FName StartSectionName = NAME_None);


private:
	bool bLShift = true;
	float LerpAlpha = 0.0f;
	float Duration = 5.0f;


	public:
	float SlopeForwardAngle = 0.0f;
};
