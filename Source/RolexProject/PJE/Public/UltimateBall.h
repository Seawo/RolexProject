// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UltimateBall.generated.h"

UCLASS()
class ROLEXPROJECT_API AUltimateBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUltimateBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable)
	void Server_ActorPos(FVector pos);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_ActorPos(FVector pos);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	class USphereComponent* Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	UStaticMeshComponent* MeshComponent;
	
	// to bounce easily
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UProjectileMovementComponent* ProjectileMovement;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Type = true;		// true: Heal ball, false: Attack ball

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* HealBallMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* AttackBallMaterial;
	
public:
	class ABaseCharacter* OwnCharacter;
	
	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent*OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
		);
	
};
