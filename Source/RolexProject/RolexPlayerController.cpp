// Fill out your copyright notice in the Description page of Project Settings.


#include "RolexPlayerController.h"

#include "HeroSlotUI.h"
#include "PlayerSlotUI.h"
#include "WaitingRoomGameModeBase.h"
#include "WaitingRoomGameStateBase.h"
#include "WaitingRoomUI.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


ARolexPlayerController::ARolexPlayerController()
{
}

void ARolexPlayerController::BeginPlay()
{
	Super::BeginPlay();

	WaitingRoomGameStateBase = Cast<AWaitingRoomGameStateBase>(GetWorld()->GetGameState()); 
}

void ARolexPlayerController::ServerRPC_SetPlayerHeroImage_Implementation(UTexture2D* PlayerHeroTexture, int32 Index)
{
	// cannot implement MulticastRPC_SetPlayerHeroImage in RolexPlayerController since clients only have its own controller
	// client GameStateBase cannot call RPC functions but server can
	WaitingRoomGameStateBase->MulticastRPC_SetPlayerHeroImage(PlayerHeroTexture, Index); 
}

void ARolexPlayerController::ServerRPC_InformClientPlayerSlotIndex_Implementation(int32 PlayerNumber, UPlayerSlotUI* PlayerSlotUI)
{
	PlayerSlotIndex = PlayerNumber;
	OwnPlayerSlot = PlayerSlotUI;
	if (PlayerSlotUI)
	{
		PlayerSlotUI->PlayerSlotIndex = PlayerNumber;
		UE_LOG(LogTemp, Warning, TEXT("Player Controller Name %s, PlayerSlotIndex Set %d "), *GetName(), PlayerSlotUI->PlayerSlotIndex);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No OwnPlayerSlot"));
	}
}


void ARolexPlayerController::ServerRPC_UpdateWholePlayerNumber_Implementation()
{
	if (WaitingRoomGameStateBase == nullptr)
	{
		WaitingRoomGameStateBase = Cast<AWaitingRoomGameStateBase>(GetWorld()->GetGameState());
	}
		UE_LOG(LogTemp, Warning, TEXT("Exist"));
		WaitingRoomGameStateBase->WholePlayerNumber += 1;
		AWaitingRoomGameModeBase* WaitingRoomGameModeBase = Cast<AWaitingRoomGameModeBase>(GetWorld()->GetAuthGameMode());
		if (WaitingRoomGameStateBase->WholePlayerNumber == WaitingRoomGameModeBase->MaxPlayersNum)
		{
			
			WaitingRoomGameStateBase->MulticastRPC_UpdateNotice("Start Matching");

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, WaitingRoomGameStateBase, &AWaitingRoomGameStateBase::MatchPlayers, 3.0f, false);
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
		OwnPlayerSlot-> PlayerSlotIndex = PlayerNumber;

		if (WaitingRoomUI->HeroButtonArray.Num() > 0)
		{
			for (UHeroSlotUI* HeroSlotButton: WaitingRoomUI->HeroButtonArray)
			{
					HeroSlotButton->OwnerPlayerSlotIndex = PlayerNumber;
					// UE_LOG(LogTemp, Warning, TEXT("Bind Delegate"));
					// HeroSlotButton->HeroSelectButtonClickedDelegate.BindUObject(
					// 	WaitingRoomGameStateBase, &AWaitingRoomGameStateBase::ServerRPC_SetPlayerHeroImage);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No HeroButtonArray"));
		}
		
		ServerRPC_InformClientPlayerSlotIndex(PlayerNumber, PlayerSlot);
	}
	
		ServerRPC_UpdateWholePlayerNumber();
}
