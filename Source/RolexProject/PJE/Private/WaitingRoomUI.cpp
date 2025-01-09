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
	}
}

void UWaitingRoomUI::AddPlayer()
{
}


