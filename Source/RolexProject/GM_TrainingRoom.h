// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_TrainingRoom.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API AGM_TrainingRoom : public AGameModeBase
{
	GENERATED_BODY()
	
	AGM_TrainingRoom();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(NetMulticast, Reliable)
	void Multi_SetPlayTimer(float DeltaTime);

public:
	void SetPlayTime(float time) { PlayTime += time; }
	float GetPlayTime() { return PlayTime; }

private:



	bool IsActiveBsePoint = false;

	float PlayTime = 0.0f;
	float BasePoint = 0.0f;

public:


	UPROPERTY(VisibleAnywhere, Category = "Info")
	TArray<class ABaseCharacter*> ATeamChracters;		// Team == true

	UPROPERTY(VisibleAnywhere, Category = "Info")
	TArray<class ABaseCharacter*> BTeamChracters;		// Team == false

	UPROPERTY(VisibleAnywhere, Category = "Info")
	TArray<class AActor_FightPoint*> Points;
};
