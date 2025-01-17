// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSelectUI.h"

#include "RolexGameInstance.h"
#include "Components/Border.h"
#include "Components/Button.h"

void UMapSelectUI::NativeConstruct()
{
	Super::NativeConstruct();

	MedivalAsia->OnClicked.AddDynamic(this, &UMapSelectUI::OnClickedMedivalAsia);
}

void UMapSelectUI::OnClickedMedivalAsia()
{
	FLinearColor SemiTransparentColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.4f);
	MedivalAsia->SetBackgroundColor(SemiTransparentColor);
	
	URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetGameInstance());
	RolexGameInstance->TravelLevel = FString("/Game/Rolex/PublicMap/PlayMap/PlayLevel?listen");
}
