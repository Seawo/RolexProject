// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerController_TrainingRoom.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API APlayerController_TrainingRoom : public APlayerController
{
	GENERATED_BODY()
	

	virtual void BeginPlay() override;

	void InitUI();


public:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUI_Zone> UI_ZoneClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	class UUI_Zone* UI_Zone;



};
