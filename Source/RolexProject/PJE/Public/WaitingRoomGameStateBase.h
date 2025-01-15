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
	UPROPERTY(Replicated)
	int32 WholePlayerNumber = 0;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	class UWaitingRoomUI* WaitingRoomUI;
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_UpdatePlayerSlotID(int32 PlayerNum, const FString& NewText);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_UpdateNotice(const FString& NewText);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_UpdatePlayerTeam(int32 PlayerSlotIndex, FLinearColor Color);
	
public:
	void MatchPlayers();

	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 CountdownTime = 5;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Countdown();	
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_StartHeroSelection();
	
	FTimerHandle CountdownTimerHandle;
	
	void StartHeroSelectionCountdown();

// public:
// 	UFUNCTION(Server, Reliable)
// 	void ServerRPC_SetPlayerHeroImage(UTexture2D* PlayerHeroTexture, int32 Index);
//
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetPlayerHeroImage(UTexture2D* PlayerHeroTexture, int32 Index);
};
