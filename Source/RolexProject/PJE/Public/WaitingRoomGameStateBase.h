// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "WaitingRoomGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API AWaitingRoomGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

public:
	class UWaitingRoomUI* WaitingRoomUI;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_UpdatePlayerSlotID(int32 PlayerNum, const FString& NewText);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Notice(const FString& NewText);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_UpdatePlayerTeam(class ARolexPlayerState* RolexPlayerState);
public:
	void MatchPlayers();
	
	void StartCountdownBeforeHeroSelection();
	
	int32 CountdownTime;

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_Countdown();	
	
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_StartHeroSelection();

	void CountdownHeroSelectionText();
};
