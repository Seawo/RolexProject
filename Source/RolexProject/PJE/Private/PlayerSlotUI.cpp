// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSlotUI.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Net/UnrealNetwork.h"


void UPlayerSlotUI::ServerRPC_SetPlayerHeroImage_Implementation(UTexture2D* PlayerHeroTexture)
{
	MulticastRPC_SetPlayerHeroImage(PlayerHeroTexture);
}

void UPlayerSlotUI::MulticastRPC_SetPlayerHeroImage_Implementation(UTexture2D* PlayerHeroTexture)
{
	PlayerHeroImage->SetBrushFromTexture(PlayerHeroTexture);
}

void UPlayerSlotUI::OnRep_SetPlayerID()
{
	PlayerID->SetText(FText::FromString(PlayerIDString));
	UE_LOG(LogTemp, Display, TEXT("OnRep_SetPlayerID"));
}

void UPlayerSlotUI::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerSlotUI, PlayerIDString);
}
