// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_PlayMap_Occupation.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API AGM_PlayMap_Occupation : public AGameModeBase
{
	GENERATED_BODY()
	
	AGM_PlayMap_Occupation();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;



private:
	// PlayerController 가져오기
	class ARolexPlayerController* PC;
	// GameState
	//class AGS_PlayMap_Occupation* GS;

	bool IsActivePoint = false;

	float PlayTime = 0.0f;
	float BasePoint = 0.0f;


};
