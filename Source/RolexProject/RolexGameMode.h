// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RolexGameMode.generated.h"

UCLASS(minimalapi)
class ARolexGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARolexGameMode();

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
};



