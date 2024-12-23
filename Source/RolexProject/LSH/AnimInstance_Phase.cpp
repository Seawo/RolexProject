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

void UAnimInstance_Phase::AnimNotify_Attack1Spawn()
{
	UE_LOG(LogTemp, Warning, TEXT("AnimNotify_Attack1Spawn"));
	Phase->SpawnEffect("FX_Hand_R4");
}

void UAnimInstance_Phase::AnimNotify_Attack2Spawn()
{
	UE_LOG(LogTemp, Warning, TEXT("AnimNotify_Attack2Spawn"));
	Phase->SpawnEffect("FX_Hand_L4");
}

void UAnimInstance_Phase::AnimNotify_Attack3Spawn()
{
	UE_LOG(LogTemp, Warning, TEXT("AnimNotify_Attack3Spawn"));
	Phase->SpawnEffect("FX_Hand_R4");
}

void UAnimInstance_Phase::AnimNotify_Attack4Spawn()
{
	UE_LOG(LogTemp, Warning, TEXT("AnimNotify_Attack4Spawn"));
	Phase->SpawnEffect("FX_Hand_L4");
}