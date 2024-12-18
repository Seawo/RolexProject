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

	virtual void ChangeState(EAttackState state) override;
	virtual void InputAttack(const struct FInputActionValue& inputValue) override;

public:
	bool bIsRun = true;

private:
	void InputJump();
	void InputRun();
};
