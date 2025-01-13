// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroSlotUI.h"

#include "PlayerSlotUI.h"
#include "RolexGameInstance.h"
#include "RolexPlayerController.h"
#include "RolexPlayerState.h"
#include "RolexPlayerState.h"
#include "WaitingRoomGameModeBase.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UHeroSlotUI::NativeConstruct()
{
	Super::NativeConstruct();

	HeroSelectButton->OnClicked.AddDynamic(this, &UHeroSlotUI::OnHeroSelectButtonClicked);

	ARolexPlayerController* RolexPlayerController = Cast<ARolexPlayerController>(GetOwningPlayer());
	UPlayerSlotUI* PlayerSlotUI = RolexPlayerController->OwnPlayerSlot;
	if (PlayerSlotUI)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroSlotUI Bind Delegate"));
		HeroSelectButtonClickedDelegate.BindUObject(PlayerSlotUI, &UPlayerSlotUI::ServerRPC_SetPlayerHeroImage);
	}
}

void UHeroSlotUI::OnHeroSelectButtonClicked()
{
	//  set image on player slot in waiting room UI  
	HeroSelectButtonClickedDelegate.ExecuteIfBound(HeroTexture);
	
	//  map player and pawn class for the main level 
	ARolexPlayerState* RolexPlayerState = Cast<ARolexPlayerState>(GetOwningPlayerState());
	if (RolexPlayerState)
	{
		RolexPlayerState->SelectedHero = BaseCharacter;
		RolexPlayerState->FindUniqueID();
		
		URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetGameInstance());
		if (RolexGameInstance)
		{
			RolexGameInstance->PlayerHeroSelections.Add(RolexPlayerState->UniqueID, BaseCharacter);

			UE_LOG(LogTemp, Warning,TEXT("PlayerHeroSelections: %d"), RolexGameInstance->PlayerHeroSelections.Num());

			// debug
			for (auto Pair : RolexGameInstance->PlayerHeroSelections)
			{
				if (FString Mapkey = Pair.Key; *Mapkey)
					UE_LOG(LogTemp, Warning, TEXT("%s"), *Mapkey);
				
				if (TSubclassOf<ABaseCharacter> MapValue = Pair.Value)
					UE_LOG(LogTemp, Warning, TEXT("%s"), *MapValue->GetName());
			}
		}
	}
}
