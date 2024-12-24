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
	virtual void UpdateLocation(float DeltaTime) PURE_VIRTUAL (AActor_Effect::UpdateLocation, );


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDirection(FVector dir) { Direction = dir; }
	void SetSpeed(float speed) { Speed = speed; }







public:

	UPROPERTY(EditAnywhere, Category = "Effect")
	class UNiagaraComponent* NiagaraComponent;

	UPROPERTY(EditAnywhere, Category = "Effect")
	class USphereComponent* ColComp;

protected:
	FVector Direction;

	UPROPERTY(EditAnywhere)
	float Speed = 2500.0f;

	UPROPERTY(EditAnywhere)
	FName SkillName = "LMBRMB";

	UPROPERTY()
	class ACharacter_Phase* Phase;
};
