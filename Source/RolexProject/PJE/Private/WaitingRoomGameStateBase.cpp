// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomGameStateBase.h"

#include "HeroSlotUI.h"
#include "PlayerSlotUI.h"
#include "RolexGameInstance.h"
#include "RolexPlayerController.h"
#include "RolexPlayerState.h"
#include "WaitingPlayerSlotUI.h"
#include "WaitingRoomUI.h"
#include "Algo/RandomShuffle.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
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
	DOREPLIFETIME(AWaitingRoomGameStateBase, HeroSelectionCountdownTime);
}

void AWaitingRoomGameStateBase::MulticastRPC_UpdateWaitingPlayerSlotID_Implementation(
	const FString& NewText)
{
	UWaitingPlayerSlotUI* WaitingPlayerSlotUI = Cast<UWaitingPlayerSlotUI>(CreateWidget(GetWorld(), WaitingPlayerSlotUIFactory));
	if (WaitingPlayerSlotUI)
	{
		WaitingPlayerSlotUI->PlayerID->SetText(FText::FromString(NewText));
		if (WaitingRoomUI && WaitingRoomUI->WaitingPlayersBox)
			WaitingRoomUI->WaitingPlayersBox->AddChild(WaitingPlayerSlotUI);
	}
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
		WaitingRoomUI->PlayerSlots[PlayerSlotIndex]->TeamColor->SetColorAndOpacity(Color);
	}
}

// team matching
void AWaitingRoomGameStateBase::MatchPlayers()
{
	// does not make random	
	// FRandomStream Random;
	// Random.GenerateNewSeed();

	URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetGameInstance());
	
	Algo::RandomShuffle(PlayerArray);
	UE_LOG(LogTemp, Warning, TEXT("Total player number: %d"), PlayerArray.Num());
	int32 HalfPlayers = PlayerArray.Num()/2;
	
	for (int32 i = 0; i < PlayerArray.Num(); i++)
	{
		if (ARolexPlayerState* RolexPlayerState = Cast<ARolexPlayerState>(PlayerArray[i]))
		{
			RolexPlayerState->FindUniqueID();
			if (i < HalfPlayers)	// team A, true, Red
			{
				RolexPlayerState->Team = true;
				if (ARolexPlayerController* RolexPlayerController = Cast<ARolexPlayerController>(RolexPlayerState->GetPlayerController()))
				{
					RolexGameInstance->PlayerTeam.Add(RolexPlayerState->UniqueID, true);
					MulticastRPC_UpdatePlayerTeam(RolexPlayerController->PlayerSlotIndex, FLinearColor::Red);
				}
			}
			else		// team B, false, Blue
			{
				RolexPlayerState->Team = false;
				if (ARolexPlayerController* RolexPlayerController = Cast<ARolexPlayerController>(RolexPlayerState->GetPlayerController()))
				{
					RolexGameInstance->PlayerTeam.Add(RolexPlayerState->UniqueID, false);
					MulticastRPC_UpdatePlayerTeam(RolexPlayerController->PlayerSlotIndex, FLinearColor::Blue);
				}
			}
		}
	}

	WaitingRoomUI->Notice->SetVisibility(ESlateVisibility::Hidden);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AWaitingRoomGameStateBase::StartHeroSelectionCountdown, 3.0f, false);
}

void AWaitingRoomGameStateBase::StartHeroSelectionCountdown()
{
	MulticastRPC_UpdateNotice("HERO SELECTION");
	
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &AWaitingRoomGameStateBase::MulticastRPC_Countdown, 1.0f, true);
}

void AWaitingRoomGameStateBase::SelcetRandomHero()
{
	ARolexPlayerController* RolexPlayerController = Cast<ARolexPlayerController>(GetWorld()->GetFirstPlayerController());
	if (RolexPlayerController)
	{
		if  (RolexPlayerController->OwnPlayerSlot->bSelectHero) return;	// already selected a hero
	}
	
	TArray<UHeroSlotUI*> RemainHeroArray;
	for (int32 i = 0; i < WaitingRoomUI->HeroButtonArray.Num(); i++)
	{
		if (!WaitingRoomUI->HeroButtonArray[i]->AlreadySelected)
		{
			RemainHeroArray.Add(WaitingRoomUI->HeroButtonArray[i]);
		}
	}
	
	Algo::RandomShuffle(RemainHeroArray);
	
	ARolexPlayerState* RolexPlayerState = Cast<ARolexPlayerState>(RolexPlayerController->PlayerState);
	if (RolexPlayerState)
	{
		RolexPlayerState->SelectedHero = RemainHeroArray[0]->BaseCharacter;
		RolexPlayerState->FindUniqueID();
	}

	RolexPlayerController->OwnPlayerSlot->bSelectHero = true;
	
	// set the selected hero image to its own player slot
	RolexPlayerController->ServerRPC_SetPlayerSlotHeroImage(RemainHeroArray[0]->HeroTexture, RemainHeroArray[0]->OwnerPlayerSlotIndex);

	// block the hero being selected again from the other players in the same team
	RolexPlayerController->ServerRPC_BlockHero(RemainHeroArray[0]->HeroSlotIndex, RemainHeroArray[0]->OwnerPlayerSlotIndex);
	
	//  add information of "player - selected hero" pair to the server
	RolexPlayerController->ServerRPC_SetSelectedHero(RolexPlayerState->UniqueID, RemainHeroArray[0]->BaseCharacter);
}

void AWaitingRoomGameStateBase::MulticastRPC_BlockHero_Implementation(int32 HeroIndex, int32 PlayerIndex)
{
	ARolexPlayerController* RolexPlayerController = Cast<ARolexPlayerController>(GetWorld()->GetFirstPlayerController());
	UHeroSlotUI* HeroSlotUI = WaitingRoomUI->HeroButtonArray[HeroIndex];
	// if another player from the same team selected the specific Hero, it cannot be selected again
	if (HeroSlotUI)
	{
		if (RolexPlayerController->OwnPlayerSlot->TeamColor->GetColorAndOpacity() == WaitingRoomUI->PlayerSlots[PlayerIndex]->TeamColor->GetColorAndOpacity())
		{
			HeroSlotUI->AlreadySelected = true;
			HeroSlotUI->HeroImage->SetColorAndOpacity(FLinearColor(0.0, 0.0, 0.0, 0.4));
		}
	}
}

void AWaitingRoomGameStateBase::MulticastRPC_UnBlockHero_Implementation(int32 HeroIndex, int32 PlayerIndex)
{
	ARolexPlayerController* RolexPlayerController = Cast<ARolexPlayerController>(GetWorld()->GetFirstPlayerController());
	UHeroSlotUI* HeroSlotUI = WaitingRoomUI->HeroButtonArray[HeroIndex];
	if (HeroSlotUI)
	{
		if (RolexPlayerController->OwnPlayerSlot->TeamColor->GetColorAndOpacity() == WaitingRoomUI->PlayerSlots[PlayerIndex]->TeamColor->GetColorAndOpacity())
		{
			HeroSlotUI->AlreadySelected = false;
			HeroSlotUI->HeroImage->SetColorAndOpacity(FLinearColor(1.0, 1.0, 1.0, 1.0));
			HeroSlotUI->HeroImage->SetBrushFromTexture(HeroSlotUI->HeroTexture);
		}
	}
}

void AWaitingRoomGameStateBase::MulticastRPC_SetPlayerHeroImage_Implementation(UTexture2D* PlayerHeroTexture, int32 PlayerIndex)
{
	ARolexPlayerController* RolexPlayerController = Cast<ARolexPlayerController>(GetWorld()->GetFirstPlayerController());

	// make invisible if opponent from the other team selected
	if (RolexPlayerController->OwnPlayerSlot->TeamColor->GetColorAndOpacity() != WaitingRoomUI->PlayerSlots[PlayerIndex]->TeamColor->GetColorAndOpacity())
	{
		return;
	}
	
	WaitingRoomUI->PlayerSlots[PlayerIndex]->PlayerHeroImage->SetColorAndOpacity(FLinearColor(1.0, 1.0, 1.0, 1.0));
	WaitingRoomUI->PlayerSlots[PlayerIndex]->PlayerHeroImage->SetBrushFromTexture(PlayerHeroTexture);
}

void AWaitingRoomGameStateBase::MulticastRPC_Countdown_Implementation()
{
	if (!WaitingRoomUI->HeroSelectionVisible)
	{
		WaitingRoomUI->HeroSelectionPanel->SetVisibility(ESlateVisibility::Visible);
		WaitingRoomUI->HeroSelectionVisible = true;
	}
	
	if (HeroSelectionCountdownTime <= 0)
	{
		WaitingRoomUI->CountDown->SetVisibility(ESlateVisibility::Hidden);		
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);

		// if a player didn't select any hero, select randomly
		SelcetRandomHero();

		if (HasAuthority())
			WaitingRoomUI->TravelToMain();
	}
	
	WaitingRoomUI->CountDown->SetVisibility(ESlateVisibility::Visible);
	WaitingRoomUI->CountDown->SetText(FText::AsNumber(HeroSelectionCountdownTime));

	HeroSelectionCountdownTime--;
}

void AWaitingRoomGameStateBase::MulticastRPC_StartHeroSelection_Implementation()
{
	WaitingRoomUI->WaitingRoomWidgetSwitcher->SetActiveWidgetIndex(1);

	MulticastRPC_UpdateNotice("START MATCHING");

	if (HasAuthority())
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AWaitingRoomGameStateBase::MatchPlayers, 3.0f, false);
	}
}

