// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomGameStateBase.h"

#include "PlayerSlotUI.h"
#include "RolexPlayerController.h"
#include "RolexPlayerState.h"
#include "WaitingRoomUI.h"
#include "Algo/RandomShuffle.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Net/UnrealNetwork.h"


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

void AWaitingRoomGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AWaitingRoomGameStateBase, WholePlayerNumber);
	DOREPLIFETIME(AWaitingRoomGameStateBase, CountdownTime);
}

// update new player ID on slot 
void AWaitingRoomGameStateBase::MulticastRPC_UpdatePlayerSlotID_Implementation(int32 PlayerNum, const FString& NewText)
{
	if (WaitingRoomUI)
		WaitingRoomUI->PlayerSlots[PlayerNum]->PlayerID->SetText(FText::FromString(NewText));
}

void AWaitingRoomGameStateBase::MulticastRPC_UpdateNotice_Implementation(const FString& NewText)
{
	if (WaitingRoomUI)
	{
		WaitingRoomUI->Notice->SetText(FText::FromString(NewText));
		WaitingRoomUI->Notice->SetVisibility(ESlateVisibility::Visible);
	}
}

void AWaitingRoomGameStateBase::MulticastRPC_UpdatePlayerTeam_Implementation(int32 PlayerSlotIndex, FLinearColor Color)
{
	if (WaitingRoomUI)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerSlotIndex: %d"), PlayerSlotIndex);
		WaitingRoomUI->PlayerSlots[PlayerSlotIndex]->TeamColor->SetColorAndOpacity(Color);
	}
}

void AWaitingRoomGameStateBase::MatchPlayers()
{
	// does not make random	
	// FRandomStream Random;
	// Random.GenerateNewSeed();
	
	Algo::RandomShuffle(PlayerArray);
	int32 HalfPlayers = PlayerArray.Num()/2;
	for (int32 i = 0; i < PlayerArray.Num(); i++)
	{
		ARolexPlayerState* RolexPlayerState = Cast<ARolexPlayerState>(PlayerArray[i]);
		if (RolexPlayerState)
		{
			UE_LOG(LogTemp, Warning, TEXT("Rolex Player State Exists, %d player"), i);
			if (i < HalfPlayers)
			{
				RolexPlayerState->Team = true;
				ARolexPlayerController* RolexPlayerController = Cast<ARolexPlayerController>(RolexPlayerState->GetPlayerController());
				if (RolexPlayerController)
				{
					UE_LOG(LogTemp, Warning, TEXT("Rolex Player Controller Exists, Player %d, Name: %s, PlayerSlotIndex: %d"),
						i, *RolexPlayerController->GetName(), RolexPlayerController->PlayerSlotIndex);
					MulticastRPC_UpdatePlayerTeam(RolexPlayerController->PlayerSlotIndex, FLinearColor::Red);
				}
			}
			else
			{
				RolexPlayerState->Team = false;
				ARolexPlayerController* RolexPlayerController = Cast<ARolexPlayerController>(RolexPlayerState->GetPlayerController());
				if (RolexPlayerController)
				{
					UE_LOG(LogTemp, Warning, TEXT("Rolex Player Controller Exists, %d player, Name: %s"), i, *RolexPlayerController->GetName());
					MulticastRPC_UpdatePlayerTeam(RolexPlayerController->PlayerSlotIndex, FLinearColor::Blue);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Rolex Player State Not Exists, %d player"), i);
		}
	}

	WaitingRoomUI->Notice->SetVisibility(ESlateVisibility::Hidden);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AWaitingRoomGameStateBase::StartHeroSelectionCountdown, 3.0f, false);
}

void AWaitingRoomGameStateBase::StartHeroSelectionCountdown()
{
	MulticastRPC_UpdateNotice("Hero Selection");

	CountdownTime = 15;
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &AWaitingRoomGameStateBase::MulticastRPC_Countdown, 1.0f, true);
}


void AWaitingRoomGameStateBase::MulticastRPC_Countdown_Implementation()
{
	if (CountdownTime == 15)
		WaitingRoomUI->HeroSelectionPanel->SetVisibility(ESlateVisibility::Visible);
	
	if (CountdownTime <= 0)
	{
		WaitingRoomUI->CountDown->SetVisibility(ESlateVisibility::Hidden);		
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
	}
	
	WaitingRoomUI->CountDown->SetVisibility(ESlateVisibility::Visible);
	WaitingRoomUI->CountDown->SetText(FText::AsNumber(CountdownTime));

	CountdownTime--;
	UE_LOG(LogTemp, Warning, TEXT("Countdown Time: %d"), CountdownTime);
}

void AWaitingRoomGameStateBase::MulticastRPC_StartHeroSelection_Implementation()
{
}

