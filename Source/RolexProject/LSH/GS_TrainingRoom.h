// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GS_TrainingRoom.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API AGS_TrainingRoom : public AGameStateBase
{
	GENERATED_BODY()
	
	AGS_TrainingRoom();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(NetMulticast, Reliable)
	void SetTimer(float DeltaTime);


	class AGM_TrainingRoom* GM;

	bool IsActiveBsePoint = false;
};
