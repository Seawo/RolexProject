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
	
	//  set image on player slot in waiting room UI // index can be Zero
	UE_LOG(LogTemp, Warning, TEXT("OwnerPlayerSlotIndex: %d"), OwnerPlayerSlotIndex);

	//HeroSelectButtonClickedDelegate.ExecuteIfBound(HeroTexture, OwnerPlayerSlotIndex);
	RolexPlayerController->ServerRPC_SetPlayerHeroImage(HeroTexture, OwnerPlayerSlotIndex);
	
	//  map <player, pawn class> for the main level 
	ARolexPlayerState* RolexPlayerState = Cast<ARolexPlayerState>(GetOwningPlayerState());
	if (RolexPlayerState)
	{
		RolexPlayerState->SelectedHero = BaseCharacter;
		RolexPlayerState->FindUniqueID();
		
		URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetGameInstance());
		if (RolexGameInstance)
		{
			RolexGameInstance->PlayerHeroSelections.Add(RolexPlayerState->UniqueID, BaseCharacter);

			//UE_LOG(LogTemp, Warning,TEXT("PlayerHeroSelections: %d"), RolexGameInstance->PlayerHeroSelections.Num());

			// // debug
			// for (auto Pair : RolexGameInstance->PlayerHeroSelections)
			// {
			// 	if (FString Mapkey = Pair.Key; *Mapkey)
			// 		UE_LOG(LogTemp, Warning, TEXT("%s"), *Mapkey);
			// 	
			// 	if (TSubclassOf<ABaseCharacter> MapValue = Pair.Value)
			// 		UE_LOG(LogTemp, Warning, TEXT("%s"), *MapValue->GetName());
			// }
		}
	}
}
