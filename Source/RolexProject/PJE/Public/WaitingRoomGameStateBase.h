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
	
	class UWaitingRoomUI* WaitingRoomUI;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_UpdatePlayerSlotID(int32 PlayerNum, const FString& NewText);
	
	void StartCountdownBeforeHeroSelection();
	
	// UPROPERTY(ReplicatedUsing=OnRep_Countdown)
	// int32 CountdownTime;

	UFUNCTION()
	void OnRep_Countdown();

	
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_CountdownHeroSelection();	
	
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRPC_StartHeroSelection();

	void CountdownHeroSelectionText();
};
