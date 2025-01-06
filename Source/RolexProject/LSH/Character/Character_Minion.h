// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Character_Minion.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API ACharacter_Minion : public ABaseCharacter
{
	GENERATED_BODY()
	
	ACharacter_Minion();

	// 기본 함수
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


};
