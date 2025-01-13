// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomUI.h"

#include "PlayerSlotUI.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Net/UnrealNetwork.h"

void UWaitingRoomUI::NativeConstruct()
{
	Super::NativeConstruct();

	StartButton->OnClicked.AddDynamic(this, &UWaitingRoomUI::TravelToMain);
	
	// get player slots from player horizontal box
	if (PlayerBox)
	{
		TArray<UWidget*> PlayerBoxChildren =  PlayerBox->GetAllChildren();

		for (UWidget* PlayerBoxChild : PlayerBoxChildren)
		{
			if (UPlayerSlotUI* PlayerVerticalBox = Cast<UPlayerSlotUI>(PlayerBoxChild))
				PlayerSlots.Add(PlayerVerticalBox);
		}
		
		NumplayerSlots = PlayerSlots.Num();
		 UE_LOG(LogTemp, Warning, TEXT("Number of Player Slots: %d"), NumplayerSlots);
	}
}

void UWaitingRoomUI::AddPlayer()
{
	
}

void UWaitingRoomUI::TravelToMain()
{
	GetWorld()->ServerTravel(TEXT("/Game/Rolex/Map/Main?listen"));
}

