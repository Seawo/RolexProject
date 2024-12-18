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


	virtual void QSkill() override;
	virtual void ESkill() override;
	virtual void LMB() override;
	virtual void RMB() override;
};
