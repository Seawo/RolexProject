// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RolexPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API ARolexPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	TSubclassOf<class ABaseCharacter> SelectedHero;

	virtual void CopyProperties(APlayerState* PlayerState) override;
	
};
