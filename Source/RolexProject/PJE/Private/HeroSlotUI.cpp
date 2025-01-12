// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroSlotUI.h"

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

	AWaitingRoomGameModeBase* WaitingRoomGameModeBase = GetWorld()->GetAuthGameMode<AWaitingRoomGameModeBase>();
	if (WaitingRoomGameModeBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroSlotUI BindDelegate"));
		HeroSelectButtonClickedDelegate.BindUObject(WaitingRoomGameModeBase, &AWaitingRoomGameModeBase::SetPlayerSlotImage);
	}
}

void UHeroSlotUI::OnHeroSelectButtonClicked()
{
	//  set image on player slot in waiting room UI  
	HeroSelectButtonClickedDelegate.ExecuteIfBound(HeroTexture);
	
	//  set default pawn class
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

			for (auto Pair : RolexGameInstance->PlayerHeroSelections)
			{
				FString Mapkey = Pair.Key;
				if (*Mapkey)
					UE_LOG(LogTemp, Warning, TEXT("%s"), *Mapkey);
				TSubclassOf<ABaseCharacter> MapValue = Pair.Value;
				if (MapValue)
					UE_LOG(LogTemp, Warning, TEXT("%s"), *MapValue->GetName());
			}
		}
	}
}
