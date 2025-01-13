// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomGameStateBase.h"

#include "PlayerSlotUI.h"
#include "RolexPlayerController.h"
#include "RolexPlayerState.h"
#include "WaitingRoomUI.h"
#include "Algo/RandomShuffle.h"
#include "Components/TextBlock.h"


void AWaitingRoomGameStateBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetWorld())
	{
		ARolexPlayerController* RolexPlayerController = Cast<ARolexPlayerController>(GetWorld()->GetFirstPlayerController());
		if (RolexPlayerController)
			WaitingRoomUI = RolexPlayerController->WaitingRoomUI;
	}
}

void AWaitingRoomGameStateBase::MulticastRPC_UpdatePlayerSlotID_Implementation(int32 PlayerNum, const FString& NewText)
{
	WaitingRoomUI->PlayerSlots[PlayerNum]->PlayerID->SetText(FText::FromString(NewText));
}

void AWaitingRoomGameStateBase::MulticastRPC_Notice_Implementation(const FString& NewText)
{
	WaitingRoomUI->Notice->SetText(FText::FromString(NewText));
	WaitingRoomUI->Notice->SetVisibility(ESlateVisibility::Visible);
}

void AWaitingRoomGameStateBase::MulticastRPC_UpdatePlayerTeam_Implementation(ARolexPlayerState* RolexPlayerState)
{

}

void AWaitingRoomGameStateBase::MatchPlayers()
{
	FRandomStream Random;
	Random.GenerateNewSeed();
	
	Algo::RandomShuffle(PlayerArray);
	int32 HalfPlayers = PlayerArray.Num()/2;
	for (int32 i = 0; i < PlayerArray.Num(); i++)
	{
		ARolexPlayerState* RolexPlayerState = Cast<ARolexPlayerState>(PlayerArray[i]);
		if (i < HalfPlayers)
		{
			RolexPlayerState->Team = true;
			// ARolexPlayerController* RolexPlayerController = Cast<ARolexPlayerController>(RolexPlayerState->GetPlayerController());
			// if (RolexPlayerController)
			// {
			// 	RolexPlayerController->OwnPlayerSlot->TeamColor = 
			// }
		}
		else
		{
			RolexPlayerState->Team = false;
		}
	}
}

void AWaitingRoomGameStateBase::StartCountdownBeforeHeroSelection()
{
	FTimerHandle CountdownTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle,
		this, &AWaitingRoomGameStateBase::MultiCastRPC_Countdown, 1.0f, true);
}

void AWaitingRoomGameStateBase::MultiCastRPC_Countdown_Implementation()
{
}

void AWaitingRoomGameStateBase::MultiCastRPC_StartHeroSelection_Implementation()
{
}


void AWaitingRoomGameStateBase::CountdownHeroSelectionText()
{
}
