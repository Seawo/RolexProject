// Fill out your copyright notice in the Description page of Project Settings.


#include "RolexPlayerController.h"

#include "PlayerSlotUI.h"
#include "WaitingRoomUI.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"


ARolexPlayerController::ARolexPlayerController()
{
	
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

// init previous information (players who was already in the game)

void ARolexPlayerController::ClientRPC_SetPlayerSlotUI_Implementation(int32 PlayerNumber)
{
	// set Steam ID on player slot textbox
	UPlayerSlotUI* PlayerSlot = WaitingRoomUI->PlayerSlots[PlayerNumber];
	if (PlayerSlot)
	{
		OwnPlayerSlot = PlayerSlot;
		UE_LOG(LogTemp, Warning, TEXT("Set PlayerIDString"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Set PlayerIDString Failed"));
	}
}