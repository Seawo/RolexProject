// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroSlotUI.h"

#include "CharacterCamera.h"
#include "Character_Minion.h"
#include "PlayerSlotUI.h"
#include "RolexGameInstance.h"
#include "RolexPlayerController.h"
#include "RolexPlayerState.h"
#include "RolexPlayerState.h"
#include "WaitingRoomGameModeBase.h"
#include "WaitingRoomGameStateBase.h"
#include "WaitingRoomUI.h"
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

	CharacterCamera = Cast<ACharacterCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterCamera::StaticClass()));
}

void UHeroSlotUI::OnHeroSelectButtonClicked()
{
	// if someone in the same team already selected this hero, return
	if (AlreadySelected) return;

	// if the player want to change the hero
	if (RolexPlayerController->OwnPlayerSlot->bSelectHero == true)
	{
		// unblock the previous hero and set the previous hero texture
		RolexPlayerController->ServerRPC_UnBlockHero(RolexPlayerController->PreviousSelectedHeroSlotIndex, OwnerPlayerSlotIndex);
	}

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

	RolexPlayerController->OwnPlayerSlot->bSelectHero = true;

	// set the selected hero image to its own player slot
	RolexPlayerController->ServerRPC_SetPlayerSlotHeroImage(HeroTexture, OwnerPlayerSlotIndex);

	// block the hero being selected again from the other players in the same team
	RolexPlayerController->ServerRPC_BlockHero(HeroSlotIndex, OwnerPlayerSlotIndex);
	
	//  add information of "player - selected hero" pair to the server
	RolexPlayerController->ServerRPC_SetSelectedHero(RolexPlayerState->UniqueID, BaseCharacter);
	//  map <player, pawn class> for the main level 

	RolexPlayerController->PreviousSelectedHeroSlotIndex = HeroSlotIndex;

	// set the hero image on the background
	if (BaseCharacter)
	{
		// CDO class default object
		ABaseCharacter* DefaultCharacter = BaseCharacter->GetDefaultObject<ABaseCharacter>();
		if (DefaultCharacter)
		{
			// get mesh asset from hero slot UI
			CharacterSkeletalMeshAsset = DefaultCharacter->GetMesh()->GetSkeletalMeshAsset();
			//CharacterCamera->SkeletalMesh->SetSkeletalMesh(CharacterSkeletalMesh);
			UE_LOG(LogTemp, Warning, TEXT("Character Mesh Asset: %s"), *CharacterSkeletalMeshAsset->GetName());

			// set mesh asset for character camera
			CharacterCamera->CharacterSkeletalMeshAsset = CharacterSkeletalMeshAsset;
			UE_LOG(LogTemp, Warning, TEXT("CharacterCamera Character Mesh Asset: %s"), *CharacterSkeletalMeshAsset->GetName());
		}
	}
}
