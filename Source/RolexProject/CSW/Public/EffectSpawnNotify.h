// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EffectSpawnNotify.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API UEffectSpawnNotify : public UAnimNotify
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TSubclassOf<class AEffectActor> EffectActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UNiagaraSystem* NiagaraEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UParticleSystem* ParticleEffect;


	// 스폰 위치( 자신의 위치에서 어느정도로 할지 )
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FVector LocationOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FRotator RotationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FVector EffectScale = FVector(1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float collionRang = 100.0f;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
