// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomGameModeBase.h"
#include "WaitingRoomUI.h"
#include "Components/VerticalBox.h"

void AWaitingRoomGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (WaitingRoomUIFactory)
	{
		UWaitingRoomUI* WaitingRoomUI = Cast<UWaitingRoomUI>(CreateWidget(GetWorld(), WaitingRoomUIFactory));
		WaitingRoomUI->AddToViewport();
	}
}

void AWaitingRoomGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (NewPlayer)
	{
		ULocalPlayer* LocalPlayer =NewPlayer->GetLocalPlayer();
		if (LocalPlayer)
		{
			FUniqueNetIdPtr NetId = LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();
			FString SteamId = NetId->ToString();

			if (*SteamId)
			{
				// set text
			}
		}
	}
}
