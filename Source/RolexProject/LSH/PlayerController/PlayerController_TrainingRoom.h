// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GM_TrainingRoom.h"
#include "GameFramework/PlayerController.h"
#include "RolexPlayerController.h"
#include "PlayerController_TrainingRoom.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API APlayerController_TrainingRoom : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void InitUI();

public:
	void SetPlayTime(float Time);
	void SetPoint(int idx);
	void SetTakingGuage(float Agauge, float Bgauge);

	void SetATeamCount(int32 Count);
	void SetBTeamCount(int32 Count);
	void SetPercent(EOccupation occupation, float APercent, float BPercent);

	void SetClashing(EClashing clash);
	void SetExtraTime(float Time);

	void SetIsATeamExtraTime(bool bExtra);
	void SetIsBTeamExtraTime(bool bExtra);

public:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUI_Zone> UI_ZoneClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	class UUI_Zone* UI_Zone;
};
