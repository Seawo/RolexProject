// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomUI.h"

#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"

void UWaitingRoomUI::NativeConstruct()
{
	Super::NativeConstruct();

	// get player slots from player horizontal box
	if (PlayerBox)
	{
		const TArray<UWidget*> PlayerBoxChildren =  PlayerBox->GetAllChildren();

		for (UWidget* PlayerBoxChild : PlayerBoxChildren)
		{
			if (UVerticalBox* PlayerVerticalBox = Cast<UVerticalBox>(PlayerBoxChild))
				PlayerSlots.Add(PlayerVerticalBox);
		}
		
		NumplayerSlots = PlayerSlots.Num();
		 UE_LOG(LogTemp, Warning, TEXT("Number of Player Slots: %d"), NumplayerSlots);
	}

	// get buttons from hero selection grid panel
	if (HeroSelectionPanel)
	{
		TArray<UWidget*> HeroSelectionChildren = HeroSelectionPanel->GetAllChildren();
		for (UWidget* HeroSelectionChild : HeroSelectionChildren)
		{
			if (UButton* HeroButton = Cast<UButton>(HeroSelectionChild))
			{
				// https://www.youtube.com/watch?v=sHq69iYfd9E
				// https://www.youtube.com/watch?v=OHr1G9pGLGE
				HeroButton->OnClicked.AddUniqueDynamic(this,&UWaitingRoomUI::OnHeroClicked);
				HeroButtonArray.Add(HeroButton);
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("Number of Hero: %d"), HeroButtonArray.Num());
	}
}

void UWaitingRoomUI::AddPlayer()
{
	
}

void UWaitingRoomUI::OnHeroClicked()
{
	
}


