// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "WaitingRoomGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API AWaitingRoomGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

public:
	IOnlineSubsystem* OnlineSubsystem;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UWaitingRoomUI> WaitingRoomUIFactory;
	
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
};
