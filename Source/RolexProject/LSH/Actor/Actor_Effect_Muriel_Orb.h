// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSH/Actor/Actor_Effect.h"
#include "Actor_Effect_Muriel_Orb.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API AActor_Effect_Muriel_Orb : public AActor_Effect
{
	GENERATED_BODY()

	AActor_Effect_Muriel_Orb();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void UpdateLocation(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void UpdateScale(float DeltaTime);

private:
	UPROPERTY(EditAnywhere, Category = "Effect")
	class USphereComponent* OrbCollision;

	UPROPERTY(EditAnywhere, Category = "Effect")
	class UNiagaraSystem* HitNiagaraSystem;

	FVector HitLocation1;

	class ACharacter_Muriel* Muriel;

	float OrbScale = 0.3f;
	bool bIsFire = false;
};
