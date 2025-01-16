// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSlotUI.h"

#include "RolexPlayerState.h"
#include "WaitingRoomGameModeBase.h"
#include "WaitingRoomGameStateBase.h"
#include "WaitingRoomUI.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Net/UnrealNetwork.h"




// void UPlayerSlotUI::ServerRPC_SetPlayerHeroImage_Implementation(UTexture2D* PlayerHeroTexture, int32 Index)
// {
// 	UE_LOG(LogTemp, Warning, TEXT("ServerRPC_SetPlayerHeroImage, Index: %d"), Index);
// 	MulticastRPC_SetPlayerHeroImage(PlayerHeroTexture, Index);
// }
//
// void UPlayerSlotUI::MulticastRPC_SetPlayerHeroImage_Implementation(UTexture2D* PlayerHeroTexture, int32 Index)
// {
// 	UE_LOG(LogTemp, Warning, TEXT("MulticastRPC_SetPlayerHeroImage"));
// 	AWaitingRoomGameStateBase* WaitingRoomGameStateBase = Cast<AWaitingRoomGameStateBase>(GetWorld()->GetGameState());
// 	if (WaitingRoomGameStateBase)
// 	{
// 		WaitingRoomGameStateBase->WaitingRoomUI->PlayerSlots[Index]->PlayerHeroImage->SetBrushFromTexture(PlayerHeroTexture);
// 	}
// }

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
