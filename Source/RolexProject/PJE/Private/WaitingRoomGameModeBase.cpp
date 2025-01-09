// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomGameModeBase.h"
#include "WaitingRoomUI.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "OnlineSubsystemSteam.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "ScreenPass.h"
#include "Interfaces/OnlineIdentityInterface.h"


void AWaitingRoomGameModeBase::InitializeUI()
{
	if (WaitingRoomUIFactory)
	{
		WaitingRoomUI = Cast<UWaitingRoomUI>(CreateWidget(GetWorld(), WaitingRoomUIFactory));
		if (WaitingRoomUI)
		{
			WaitingRoomUI->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("Waiting Room UI Created"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Waiting Room UI is NULL"));
		}
	}
}

void AWaitingRoomGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (WaitingRoomUI == nullptr)
		InitializeUI();
	
	if (NewPlayer)
	{
		ULocalPlayer* LocalPlayer =NewPlayer->GetLocalPlayer();
		if (LocalPlayer)
		{
			// // get Steam ID
			// FUniqueNetIdPtr NetId = LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();
			// FString SteamId = NetId->ToString();
			//
			// UE_LOG(LogTemp, Warning, TEXT("SteamId: %s"), *SteamId);
			FString SteamId = GetSteamID(NewPlayer);
			
			if (*SteamId && WaitingRoomUI)
			{
				// set Steam ID on player slot
				const TArray<UWidget*>& PlayerSlot = WaitingRoomUI->PlayerSlots[CurrentPlayersNum]->GetAllChildren();
				for (UWidget* SlotWidget: PlayerSlot)
				{
					if (UTextBlock* TextBlock = Cast<UTextBlock>(SlotWidget))
						TextBlock->SetText(FText::FromString(*SteamId));
				}
			}
		}
		CurrentPlayersNum++;
		UE_LOG(LogTemp, Warning, TEXT("Number of Current Players = %d"), CurrentPlayersNum);
	}
}

FString AWaitingRoomGameModeBase::GetSteamID(APlayerController* NewPlayer)
{
	FString SteamNickName = TEXT("unknown");
	
	OnlineSubsystem = Online::GetSubsystem(GetWorld(), "STEAM");
	if (OnlineSubsystem)
	{
		IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
		
		ULocalPlayer* LocalPlayer =NewPlayer->GetLocalPlayer();
		if (LocalPlayer)
		{
			// get Steam ID
			FUniqueNetIdPtr NetId = LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();

			// FString SteamId = NetId->ToString();
			// UE_LOG(LogTemp, Warning, TEXT("SteamId: %s"), *SteamId);
			
			SteamNickName = Identity->GetPlayerNickname(*NetId);
			
			UE_LOG(LogTemp, Warning, TEXT("Steam Nick Name: %s"), *SteamNickName);
		}
	}

	return SteamNickName;
}