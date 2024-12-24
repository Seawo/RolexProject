// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSH/Actor/Actor_Effect.h"
#include "Actor_Effect_E.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API AActor_Effect_E : public AActor_Effect
{
	GENERATED_BODY()

	AActor_Effect_E();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void UpdateLocation(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:


	UPROPERTY(EditAnywhere, Category = "Effect")
	class USphereComponent* ShieldCollision;

	bool bIsOverlap = false;
};
