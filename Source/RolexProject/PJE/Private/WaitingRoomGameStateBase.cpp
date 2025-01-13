// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomGameStateBase.h"

#include "PlayerSlotUI.h"
#include "RolexPlayerController.h"
#include "WaitingRoomUI.h"
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

void AWaitingRoomGameStateBase::StartCountdownBeforeHeroSelection()
{
	// FTimerHandle CountdownTimerHandle;
	// GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, )
}

void AWaitingRoomGameStateBase::OnRep_Countdown()
{
}

void AWaitingRoomGameStateBase::MultiCastRPC_CountdownHeroSelection_Implementation()
{
}

void AWaitingRoomGameStateBase::MultiCastRPC_StartHeroSelection_Implementation()
{
	
}


void AWaitingRoomGameStateBase::CountdownHeroSelectionText()
{
}
