// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSH/Actor/Actor_Effect.h"
#include "Actor_Effect_Q.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API AActor_Effect_Q : public AActor_Effect
{
	GENERATED_BODY()

	AActor_Effect_Q();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void UpdateLocation(float DeltaTime) override;


	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void TakeDamageToCharacter();

	void DrawLineTrace();

private:

	UPROPERTY(EditAnywhere, Category = "Effect")
	class UBoxComponent* BeamCollision;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TArray<class ABaseCharacter*> OverlappedActors;

	FTimerHandle DamageTimer;

	float LineTraceDistance = 10000.0f;
	bool bIsOverlap = false;
};
