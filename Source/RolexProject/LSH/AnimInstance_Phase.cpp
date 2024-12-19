// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/AnimInstance_Phase.h"
#include "Character_Phase.h"

void UAnimInstance_Phase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	// Get the owning pawn
	Phase = Cast<ACharacter_Phase>(TryGetPawnOwner());
}

void UAnimInstance_Phase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Phase)
	{
		SlopeForwardAngle = Phase->SlopeForwardAngle;
	}
}
