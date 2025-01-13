// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomGameModeBase.h"

#include <string>

#include "WaitingRoomUI.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "OnlineSubsystemSteam.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "PlayerSlotUI.h"
#include "RolexPlayerController.h"
#include "ScreenPass.h"
#include "WaitingRoomGameStateBase.h"
#include "Components/Image.h"
#include "Interfaces/OnlineIdentityInterface.h"


AWaitingRoomGameModeBase::AWaitingRoomGameModeBase()
{
	bUseSeamlessTravel = true;
}

// called when a new player joins the server
void AWaitingRoomGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (NewPlayer)
	{
		// get ID
		FString SteamId= GetSteamID(NewPlayer);
		PlayerIDArray.Add(SteamId);
		
		// create widget and initialize for the new player
		ARolexPlayerController* RolexPlayerController = Cast<ARolexPlayerController>(NewPlayer);
		if (RolexPlayerController)
		{
			RolexPlayerController->ClientRPC_CreateWaitingRoomUI();
			RolexPlayerController->ClientRPC_InitWaitingRoomUI(PlayerIDArray);
			RolexPlayerController->ClientRPC_SetPlayerSlotUI(CurrentPlayersNum);
		}

		// update new player for the existing players
		AWaitingRoomGameStateBase* WaitingRoomGameStateBase = Cast<AWaitingRoomGameStateBase>(GetGameState<AGameStateBase>());
		if (WaitingRoomGameStateBase)
		{
			if (WaitingRoomGameStateBase->WaitingRoomUI)
			{
				WaitingRoomGameStateBase->MulticastRPC_UpdatePlayerSlotID(CurrentPlayersNum, SteamId);
				// WaitingRoomGameStateBase->WaitingRoomUI is different by player
				// WaitingRoomGameStateBase->WaitingRoomUI->PlayerSlots[CurrentPlayersNum]->PlayerIDString = SteamId;
				// WaitingRoomGameStateBase->WaitingRoomUI->PlayerSlots[CurrentPlayersNum]->OnRep_SetPlayerID();
			}
		}
		
		CurrentPlayersNum++;
		UE_LOG(LogTemp, Warning, TEXT("Number of Current Players = %d"), CurrentPlayersNum);
	}

	// switch to hero selection stage
	if (CurrentPlayersNum == MaxPlayersNum)
	{
		AWaitingRoomGameStateBase* WaitingRoomGameStateBase = Cast<AWaitingRoomGameStateBase>(GetGameState<AGameStateBase>());
		WaitingRoomGameStateBase->MulticastRPC_Notice(TEXT("Start Matching"));
		
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, WaitingRoomGameStateBase, &AWaitingRoomGameStateBase::MatchPlayers, 2.0f, false);
	}
}

FString AWaitingRoomGameModeBase::GetSteamID(APlayerController* NewPlayer)
{
	FString SteamNickName = FString::FromInt(NewPlayer->GetUniqueID());
	
	OnlineSubsystem = Online::GetSubsystem(GetWorld(), "STEAM");
	if (OnlineSubsystem)
	{
		IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
		
		ULocalPlayer* LocalPlayer =NewPlayer->GetLocalPlayer();
		if (LocalPlayer)
		{
			// get Steam ID
			FUniqueNetIdPtr NetId = LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();
			SteamNickName = Identity->GetPlayerNickname(*NetId);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Steam Nick Name: %s"), *SteamNickName);
	return SteamNickName;
}
