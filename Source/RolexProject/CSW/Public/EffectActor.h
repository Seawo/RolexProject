// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EffectActor.generated.h"

UCLASS()
class ROLEXPROJECT_API AEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEffectActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	bool bIsDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float DestroyTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float CollisionRadius = 100.0f;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticleComp;
	
	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* NiagaraComp;

	UPROPERTY()
	class USphereComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UStaticMeshComponent* StaticMeshComp;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UNiagaraSystem* NiagaraCollusionEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UParticleSystem* ParticleCollusionEffect;



	// Initialize Niagara Effect
	UFUNCTION(BlueprintCallable, Category = "Niagara")
	void InitializeEffect(class UNiagaraSystem* niagaraSystem, FVector scale);

	// Initialize Particle Effect
	void InitializeEffect(UParticleSystem* ParticleEffect, FVector EffectScale);

	void InititalizeThrowStone(const FVector& dir, float speed);

private:
	FVector MoveDir;
	float ThrowSpeed;

	

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
