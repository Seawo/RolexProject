// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor_StartPoint.generated.h"

UCLASS()
class ROLEXPROJECT_API AActor_StartPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_StartPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	FVector SpawnPoint();



	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;
};
