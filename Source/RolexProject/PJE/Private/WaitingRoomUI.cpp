// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomUI.h"

#include "HeroSlotUI.h"
#include "PlayerSlotUI.h"
#include "RolexGameInstance.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/UniformGridPanel.h"
#include "Net/UnrealNetwork.h"

void UWaitingRoomUI::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController ->bShowMouseCursor = true;
	}
	
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

	// get Hero Buttons from Hero selection panel, detect every child (not only the first level child)
	if (HeroSelectionPanel)
	{
		GetAllDescendants(HeroSelectionPanel, HeroButtonArray);
		UE_LOG(LogTemp, Warning, TEXT("Number of Hero Buttons: %d"), HeroButtonArray.Num());
	}

	// set its own index to each hero slit 
	if (HeroButtonArray.Num() > 0)
	{
		for (int32 i = 0; i < HeroButtonArray.Num(); i++)
			HeroButtonArray[i]->HeroSlotIndex = i;
	}
}

void UWaitingRoomUI::GetAllDescendants(UPanelWidget* ParentWidget, TArray<UHeroSlotUI*>& Descendants)
{
	for (int32 i = 0; i < ParentWidget->GetChildrenCount(); i++)
	{
		UWidget* Child = ParentWidget->GetChildAt(i);
		
		if (UHeroSlotUI* HeroSlotUI = Cast<UHeroSlotUI>(Child))
		{
			Descendants.Add(HeroSlotUI);
		}

		// if  it has its own child
		if (UPanelWidget* Panel = Cast<UPanelWidget>(Child))
			GetAllDescendants(Panel, Descendants);
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
	URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetGameInstance());
	GetWorld()->ServerTravel(RolexGameInstance->TravelLevel);
}

