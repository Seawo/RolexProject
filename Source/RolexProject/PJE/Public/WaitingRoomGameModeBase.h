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

	AWaitingRoomGameModeBase();

	virtual void BeginPlay() override;
	
public:
	IOnlineSubsystem* OnlineSubsystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxPlayersNum = 4;

	int32 CurrentPlayersNum = 0;
	
	// called when a new player joins the server
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	FString GetSteamID(APlayerController* NewPlayer);

	TArray<FString> PlayerIDArray;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class ABaseCharacter*> HeroArray;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
};

