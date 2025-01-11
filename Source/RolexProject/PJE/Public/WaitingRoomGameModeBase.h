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
	
public:
	IOnlineSubsystem* OnlineSubsystem;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UWaitingRoomUI> WaitingRoomUIFactory;

	UWaitingRoomUI* WaitingRoomUI;

	void InitializeUI();
	
	int32 CurrentPlayersNum = 0;

	UPROPERTY(EditAnywhere)
	int32 MaxPlayersNum = 1;

	// called when a new player joins the server
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	FString GetSteamID(APlayerController* NewPlayer);

	UFUNCTION()
	void SetPlayerSlotImage(UTexture2D* Texture2D);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class ABaseCharacter*> HeroArray;
	
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	
};

