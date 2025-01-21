// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroSlotUI.h"

#include "PlayerSlotUI.h"
#include "RolexGameInstance.h"
#include "RolexPlayerController.h"
#include "RolexPlayerState.h"
#include "RolexPlayerState.h"
#include "WaitingRoomGameModeBase.h"
#include "WaitingRoomGameStateBase.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UHeroSlotUI::NativeConstruct()
{
	Super::NativeConstruct();

	WaitingRoomGameStateBase = Cast<AWaitingRoomGameStateBase>(GetWorld()->GetGameState());
	RolexPlayerController = Cast<ARolexPlayerController>(GetWorld()->GetFirstPlayerController());
	HeroSelectButton->OnClicked.AddDynamic(this, &UHeroSlotUI::OnHeroSelectButtonClicked);

	HeroInfo->SetText(HeroDescription);
}

void UHeroSlotUI::OnHeroSelectButtonClicked()
{
	// if someone in the same team already selected this hero, return
	if (AlreadySelected) return;
		
	if (WaitingRoomGameStateBase== nullptr)
		WaitingRoomGameStateBase = Cast<AWaitingRoomGameStateBase>(GetWorld()->GetGameState());

	if (RolexPlayerController ==nullptr)
		RolexPlayerController = Cast<ARolexPlayerController>(GetWorld()->GetFirstPlayerController());
	
	// //  set image on player slot in waiting room UI // index can be Zero
	// UE_LOG(LogTemp, Warning, TEXT("OwnerPlayerSlotIndex: %d"), OwnerPlayerSlotIndex);

	ARolexPlayerState* RolexPlayerState = Cast<ARolexPlayerState>(RolexPlayerController->PlayerState);
	if (RolexPlayerState)
	{
		RolexPlayerState->SelectedHero = BaseCharacter;
		RolexPlayerState->FindUniqueID();
		UE_LOG(LogTemp, Warning, TEXT("UniqueID: %s"), *RolexPlayerState->UniqueID);
	}

	RolexPlayerController->OwnPlayerSlot->SelectHero = true;
	
	// set the selected hero image to its own player slot
	RolexPlayerController->ServerRPC_SetPlayerSlotHeroImage(HeroTexture, OwnerPlayerSlotIndex);

	// block the hero being selected again from the other players in the same team
	RolexPlayerController->ServerRPC_BlockHero(HeroSlotIndex, OwnerPlayerSlotIndex);
	
	//  add information of "player - selected hero" pair to the server
	RolexPlayerController->ServerRPC_SetSelectedHero(RolexPlayerState->UniqueID, BaseCharacter);
	//  map <player, pawn class> for the main level 
}
