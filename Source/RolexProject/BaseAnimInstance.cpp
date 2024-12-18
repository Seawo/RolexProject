// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimInstance.h"

void UBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());

	if (baseCharacter)
	{
		MoveState = baseCharacter->MoveState;
		AttackState = baseCharacter->AttackState;
	}
}
