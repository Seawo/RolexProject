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
#include "Kismet/GameplayStatics.h"

void UHeroSlotUI::NativeConstruct()
{
	Super::NativeConstruct();

	WaitingRoomGameStateBase = Cast<AWaitingRoomGameStateBase>(GetWorld()->GetGameState());
	RolexPlayerController = Cast<ARolexPlayerController>(GetWorld()->GetFirstPlayerController());
	HeroSelectButton->OnClicked.AddDynamic(this, &UHeroSlotUI::OnHeroSelectButtonClicked);
}

void UHeroSlotUI::OnHeroSelectButtonClicked()
{
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
	
	//HeroSelectButtonClickedDelegate.ExecuteIfBound(HeroTexture, OwnerPlayerSlotIndex);
	RolexPlayerController->ServerRPC_SetPlayerHeroImage(HeroTexture, OwnerPlayerSlotIndex);
	RolexPlayerController->ServerRPC_SetSelectedHero(RolexPlayerState->UniqueID, BaseCharacter);
	//RolexPlayerController->ServerRPC_SetSelectedHero(RolexPlayerState, BaseCharacter);
	
	//  map <player, pawn class> for the main level 
}
