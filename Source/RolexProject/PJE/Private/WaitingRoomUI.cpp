// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomUI.h"

#include "HeroSlotUI.h"
#include "PlayerSlotUI.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Net/UnrealNetwork.h"

void UWaitingRoomUI::NativeConstruct()
{
	Super::NativeConstruct();

	Notice->SetVisibility(ESlateVisibility::Hidden);
	CountDown->SetVisibility(ESlateVisibility::Hidden);
	HeroSelectionPanel->SetVisibility(ESlateVisibility::Hidden);
	StartButton->SetVisibility(ESlateVisibility::Hidden);
	
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

	// get Hero Buttons from Hero selection panel
	if (HeroSelectionPanel)
	{
		TArray<UWidget*> HeroSelectionPanelChildren =  HeroSelectionPanel->GetAllChildren();

		for (UWidget* HeroSelectionPanelChild : HeroSelectionPanelChildren)
		{
			if (UHeroSlotUI* HeroSlotUI = Cast<UHeroSlotUI>(HeroSelectionPanelChild))
				HeroButtonArray.Add(HeroSlotUI);
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Number of Hero Buttons: %d"), HeroButtonArray.Num());
	}
}

void UWaitingRoomUI::AddPlayer()
{
	
}

void UWaitingRoomUI::OnRep_SetNotice()
{
	Notice->SetVisibility(ESlateVisibility::Visible);
	Notice->SetText(NoticeText);
}

void UWaitingRoomUI::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWaitingRoomUI, NoticeText);
}

void UWaitingRoomUI::TravelToMain()
{
	GetWorld()->ServerTravel(TEXT("/Game/Rolex/PublicMap/PlayMap/PlayLevel?listen"));
}

