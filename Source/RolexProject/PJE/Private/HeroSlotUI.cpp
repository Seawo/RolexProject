// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroSlotUI.h"

#include "RolexGameInstance.h"
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
		UE_LOG(LogTemp, Warning, TEXT("Called Waiting Room GameMode Base from HeroSlot UI"));
		HeroSelectButtonClickedDelegate.BindUObject(WaitingRoomGameModeBase, &AWaitingRoomGameModeBase::SetPlayerSlotImage);
	}
}

void UHeroSlotUI::OnHeroSelectButtonClicked()
{
	//  set image on player slot in waiting room UI  
	HeroSelectButtonClickedDelegate.ExecuteIfBound(HeroTexture);

	//  set default pawn class
	URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetWorld()->GetGameInstance());
	if (RolexGameInstance)
	{
		RolexGameInstance->MainLevelPawn = BaseCharacter;
	}
	
}
