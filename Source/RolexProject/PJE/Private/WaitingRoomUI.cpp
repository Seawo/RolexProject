// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomUI.h"

#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"

void UWaitingRoomUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayerBox)
	{
		const TArray<UWidget*> Children =  PlayerBox->GetAllChildren();

		for (UWidget* Child : Children)
		{
			if (UVerticalBox* VerticalBox = Cast<UVerticalBox>(Child))
				PlayerSlots.Add(VerticalBox);
		}
		
		NumplayerSlots = PlayerSlots.Num();

		 UE_LOG(LogTemp, Warning, TEXT("Number of Player Slots: %d"), NumplayerSlots);
	}
}

void UWaitingRoomUI::AddPlayer()
{
	
}


