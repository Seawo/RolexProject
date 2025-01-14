// Fill out your copyright notice in the Description page of Project Settings.


#include "RolexPlayerController.h"

#include "PlayerSlotUI.h"
#include "WaitingRoomGameModeBase.h"
#include "WaitingRoomGameStateBase.h"
#include "WaitingRoomUI.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"


ARolexPlayerController::ARolexPlayerController()
{
	
}

void ARolexPlayerController::ServerRPC_InformClientPlayerSlotIndex_Implementation(int32 PlayerNumber, UPlayerSlotUI* PlayerSlotUI)
{
	PlayerSlotIndex = PlayerNumber;
	OwnPlayerSlot = PlayerSlotUI;
}


void ARolexPlayerController::ServerRPC_UpdateWholePlayerNumber_Implementation()
{
	AWaitingRoomGameStateBase* WaitingRoomGameStateBase = Cast<AWaitingRoomGameStateBase>(GetWorld()->GetGameState());
	if (WaitingRoomGameStateBase)
	{
		WaitingRoomGameStateBase->WholePlayerNumber += 1;
		AWaitingRoomGameModeBase* WaitingRoomGameModeBase = Cast<AWaitingRoomGameModeBase>(GetWorld()->GetAuthGameMode());
		if (WaitingRoomGameStateBase->WholePlayerNumber == WaitingRoomGameModeBase->MaxPlayersNum)
		{
			
			WaitingRoomGameStateBase->MulticastRPC_UpdateNotice("Start Matching");

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, WaitingRoomGameStateBase, &AWaitingRoomGameStateBase::MatchPlayers, 3.0f, false);
		}
	}
	
	
}

void ARolexPlayerController::ClientRPC_InitWaitingRoomUI_Implementation(const TArray<FString>& IDArray)
{
	TArray<class UPlayerSlotUI*> PlayerSlots = WaitingRoomUI->PlayerSlots;
	for (int32 i = 0; i < IDArray.Num(); i++)
	{
		PlayerSlots[i]->PlayerID->SetText(FText::FromString(IDArray[i]));
	}
}

void ARolexPlayerController::ClientRPC_CreateWaitingRoomUI_Implementation()
{
	if (IsLocalController())
	{
		if (WaitingRoomUIFactory)
		{
			WaitingRoomUI = Cast<UWaitingRoomUI>(CreateWidget(GetWorld(), WaitingRoomUIFactory));
			if (WaitingRoomUI)
				WaitingRoomUI->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("Waiting Room Created"));
		}
	}
}


void ARolexPlayerController::ClientRPC_SetPlayerSlotUI_Implementation(int32 PlayerNumber)
{
	// set Steam ID on player slot textbox
	PlayerSlotIndex = PlayerNumber;
	UE_LOG(LogTemp, Warning, TEXT("Player Controller %s, PlayerSlotIndex %d"), *GetName(), PlayerSlotIndex);
	
	UPlayerSlotUI* PlayerSlot = WaitingRoomUI->PlayerSlots[PlayerNumber];
	if (PlayerSlot)
	{
		OwnPlayerSlot = PlayerSlot;
	}

	ServerRPC_InformClientPlayerSlotIndex(PlayerNumber, PlayerSlot);
	ServerRPC_UpdateWholePlayerNumber();
}