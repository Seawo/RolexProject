// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RolexPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API ARolexPlayerController : public APlayerController
{
	GENERATED_BODY()

	ARolexPlayerController();

public:
	virtual void BeginPlay() override;
	
	class AWaitingRoomGameStateBase* WaitingRoomGameStateBase;
		
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UWaitingRoomUI> WaitingRoomUIFactory;

	UWaitingRoomUI* WaitingRoomUI;

	UFUNCTION(Client, Reliable)
	void ClientRPC_CreateWaitingRoomUI();
	
	UFUNCTION(Client, Reliable)
	void ClientRPC_InitWaitingRoomUI(const TArray<FString>& IDArray);

	UFUNCTION(Client, Reliable)
	void ClientRPC_SetPlayerSlotUI(int32 PlayerNumber);

	int32 PlayerSlotIndex;
	
public:
	class UPlayerSlotUI* OwnPlayerSlot;

public:
	UFUNCTION(Server, Reliable)
	void ServerRPC_UpdateWholePlayerNumber();

	UFUNCTION(Server, Reliable)
	void ServerRPC_InformClientPlayerSlotIndex(int32 PlayerNumber, UPlayerSlotUI* PlayerSlotUI);

public:
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetPlayerHeroImage(UTexture2D* PlayerHeroTexture, int32 Index);

	// UFUNCTION(NetMulticast, Reliable)
	// void MulticastRPC_SetPlayerHeroImage(UTexture2D* PlayerHeroTexture, int32 Index);

public:
	// UFUNCTION(Server, Reliable)
	// void ServerRPC_SetSelectedHero(class ARolexPlayerState* RolexPlayerState, TSubclassOf<class ABaseCharacter> BaseCharacter);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetSelectedHero(const FString& ID, TSubclassOf<class ABaseCharacter> BaseCharacter);
};
