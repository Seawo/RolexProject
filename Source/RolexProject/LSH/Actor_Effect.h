// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor_Effect.generated.h"

UCLASS()
class ROLEXPROJECT_API AActor_Effect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_Effect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDirection(FVector dir) { Direction = dir; }
	void SetSpeed(float speed) { Speed = speed; }



	UPROPERTY(EditAnywhere, Category = "Effect")
	class UNiagaraSystem* OrbNiagaraSystem;

	UPROPERTY(EditAnywhere, Category = "Effect")
	class UNiagaraComponent* OrbNiagaraComponent;

	UPROPERTY(EditAnywhere, Category = "Effect")
	class USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere, Category = "Effect")
	class UStaticMeshComponent* MeshComp;


private:

	FVector Direction;

	UPROPERTY(EditAnywhere)
	float Speed = 2000.0f;
};
