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
#include "RolexGameInstance.h"
#include "RolexPlayerController.h"
#include "ScreenPass.h"
#include "WaitingRoomGameStateBase.h"
#include "Components/Image.h"
#include "Interfaces/OnlineIdentityInterface.h"


AWaitingRoomGameModeBase::AWaitingRoomGameModeBase()
{
	bUseSeamlessTravel = true;
}

void AWaitingRoomGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetGameInstance());
	if (RolexGameInstance->MaxPlayersNum)
	{
		MaxPlayersNum = RolexGameInstance->MaxPlayersNum;
	}
}

// function called when a new player joins the session
void AWaitingRoomGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (NewPlayer)
	{
		// get ID
		FString SteamId= GetSteamID(NewPlayer);
		PlayerIDArray.Add(SteamId);

		AWaitingRoomGameStateBase* WaitingRoomGameStateBase = Cast<AWaitingRoomGameStateBase>(GetWorld()->GetGameState());

		// hero selection part: make invisible at the first time 
		
		// create widget and initialize for the new player
		ARolexPlayerController* RolexPlayerController = Cast<ARolexPlayerController>(NewPlayer);
		if (RolexPlayerController)
		{
			RolexPlayerController->ClientRPC_CreateWaitingRoomUI();		// create UI for client
			RolexPlayerController->ClientRPC_InitWaitingRoomUI(PlayerIDArray);	// init UI content
			// let server and the owner client know information both
			RolexPlayerController->ClientRPC_SetPlayerSlotUI(CurrentPlayersNum);	// set client's player slot UI
		}

		// update new player for the every player
		if (WaitingRoomGameStateBase)
		{
			if (WaitingRoomGameStateBase)
			{
				WaitingRoomGameStateBase->MulticastRPC_UpdateWaitingPlayerSlotID(SteamId);
				WaitingRoomGameStateBase->MulticastRPC_UpdatePlayerSlotID(CurrentPlayersNum, SteamId);
				
				// WaitingRoomGameStateBase->WaitingRoomUI is different by player
				// WaitingRoomGameStateBase->WaitingRoomUI->PlayerSlots[CurrentPlayersNum]->PlayerIDString = SteamId;
				// WaitingRoomGameStateBase->WaitingRoomUI->PlayerSlots[CurrentPlayersNum]->OnRep_SetPlayerID();
			}
		}
		
		CurrentPlayersNum++;
	}
}

FString AWaitingRoomGameModeBase::GetSteamID(APlayerController* NewPlayer)
{
	FString SteamNickName = FString::FromInt(NewPlayer->GetUniqueID());
	
	OnlineSubsystem = Online::GetSubsystem(GetWorld(), "STEAM");

	APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
	if (PlayerState)
	{
		SteamNickName = PlayerState->GetPlayerName();	
	}
	
	// if (OnlineSubsystem)
	// {
	// 	const FUniqueNetIdRepl& UniqueNetId = NewPlayer->PlayerState->GetUniqueId();
	// 	SteamNickName = UniqueNetId->ToString();
	// }
	
	// if (OnlineSubsystem)
	// {
	// 	// interface for accessing identity online services
	// 	IOnlineIdentityPtr IdentityInterface = OnlineSubsystem->GetIdentityInterface();
	// 	
	// 	ULocalPlayer* LocalPlayer =NewPlayer->GetLocalPlayer();
	// 	if (LocalPlayer)
	// 	{
	// 		// get Steam ID
	// 		FUniqueNetIdPtr NetId = LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();
	// 		SteamNickName = IdentityInterface->GetPlayerNickname(*NetId);
	// 	}
	// }

	UE_LOG(LogTemp, Warning, TEXT("Steam Nick Name: %s"), *SteamNickName);
	return SteamNickName;
}

AActor* AWaitingRoomGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	return Super::ChoosePlayerStart_Implementation(Player);
}

