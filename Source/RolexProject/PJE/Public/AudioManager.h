// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AudioManager.generated.h"

UCLASS()
class ROLEXPROJECT_API AAudioManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAudioManager();

	UPROPERTY()
	UAudioComponent* AudioComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
