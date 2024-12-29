// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Animation/AnimInstance_Muriel.h"

#include "Character_Muriel.h"

void UAnimInstance_Muriel::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Muriel = Cast<ACharacter_Muriel>(TryGetPawnOwner());
}

void UAnimInstance_Muriel::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Muriel)
	{
		SlopeForwardAngle = Muriel->GetSlopeForwrdAngle();
	}
}

void UAnimInstance_Muriel::AnimNotify_RMBCharge()
{
	UE_LOG(LogTemp, Warning, TEXT("AnimNotify_RMBCharge"));
	if (Muriel->GetIsRMBCharging())
	{
		Montage_Pause();
	}
}
