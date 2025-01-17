// Fill out your copyright notice in the Description page of Project Settings.


#include "RolexPlayerController.h"

#include "HeroSlotUI.h"
#include "PlayerSlotUI.h"
#include "RolexGameInstance.h"
#include "WaitingRoomGameModeBase.h"
#include "WaitingRoomGameStateBase.h"
#include "WaitingRoomUI.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI_Zone.h"


ARolexPlayerController::ARolexPlayerController()
{
}

void ARolexPlayerController::BeginPlay()
{
	Super::BeginPlay();

	WaitingRoomGameStateBase = Cast<AWaitingRoomGameStateBase>(GetWorld()->GetGameState()); 

	if (UI_ZoneClass)
	{
		UI_Zone = CreateWidget<UUI_Zone>(this, UI_ZoneClass);
	}
}
// 서버 RPC : 플레이어가 선택한 영웅 정보를 서버에 전달
void ARolexPlayerController::ServerRPC_SetSelectedHero_Implementation(const FString& ID,
	TSubclassOf<class ABaseCharacter> BaseCharacter)
{
	// RolexGameInstance 가져오기
	URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetGameInstance());
	if (RolexGameInstance)
	{
		// PlayerHeroSelections 맵에 플레이어 ID와 선택된 영웅 클래스 추가
		RolexGameInstance->PlayerHeroSelections.Add(ID, BaseCharacter);
	
		// debug
		UE_LOG(LogTemp, Warning,TEXT("PlayerHeroSelectionsMap) Number of elements: %d"), RolexGameInstance->PlayerHeroSelections.Num());
		for (auto Pair : RolexGameInstance->PlayerHeroSelections)
		{
			if (FString Mapkey = Pair.Key; *Mapkey)
				UE_LOG(LogTemp, Warning, TEXT("Key: %s"), *Mapkey);
				
			if (TSubclassOf<ABaseCharacter> MapValue = Pair.Value)
				UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *MapValue->GetName());
		}
	}
}
// void ARolexPlayerController::ServerRPC_SetSelectedHero_Implementation(ARolexPlayerState* RolexPlayerState,
//                                                                       TSubclassOf<ABaseCharacter> BaseCharacter)
// {
// 	URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetGameInstance());
// 	if (RolexGameInstance && RolexPlayerState)
// 	{
// 		if (*RolexPlayerState->UniqueID == nullptr)
// 		{
// 			RolexPlayerState->FindUniqueID();
// 			UE_LOG(LogTemp, Warning, TEXT("Server found UniqueID: %s"), *RolexPlayerState->UniqueID);
// 		}
// 		RolexGameInstance->PlayerHeroSelections.Add(RolexPlayerState->UniqueID, BaseCharacter);
//
// 		// debug
// 		UE_LOG(LogTemp, Warning,TEXT("PlayerHeroSelectionsMap) Number of elements: %d"), RolexGameInstance->PlayerHeroSelections.Num());
// 		// for (auto Pair : RolexGameInstance->PlayerHeroSelections)
// 		// {
// 		// 	if (FString Mapkey = Pair.Key; *Mapkey)
// 		// 		UE_LOG(LogTemp, Warning, TEXT("Key: %s"), *Mapkey);
// 		// 		
// 		// 	if (TSubclassOf<ABaseCharacter> MapValue = Pair.Value)
// 		// 		UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *MapValue->GetName());
// 		// }
// 	}
// }
void ARolexPlayerController::ServerRPC_SetPlayerHeroImage_Implementation(UTexture2D* PlayerHeroTexture, int32 Index)
{
	// cannot implement MulticastRPC_SetPlayerHeroImage in RolexPlayerController since clients only have its own controller
	// client GameStateBase cannot call RPC functions but server can
	WaitingRoomGameStateBase->MulticastRPC_SetPlayerHeroImage(PlayerHeroTexture, Index); 
}
void ARolexPlayerController::ServerRPC_InformClientPlayerSlotIndex_Implementation(int32 PlayerNumber, UPlayerSlotUI* PlayerSlotUI)
{
	PlayerSlotIndex = PlayerNumber;
	OwnPlayerSlot = PlayerSlotUI;
	if (PlayerSlotUI)
	{
		PlayerSlotUI->PlayerSlotIndex = PlayerNumber;
		//UE_LOG(LogTemp, Warning, TEXT("Player Controller Name %s, PlayerSlotIndex Set %d "), *GetName(), PlayerSlotUI->PlayerSlotIndex);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No OwnPlayerSlot"));
	}
}
void ARolexPlayerController::ServerRPC_UpdateWholePlayerNumber_Implementation()
{
	if (WaitingRoomGameStateBase == nullptr)
	{
		WaitingRoomGameStateBase = Cast<AWaitingRoomGameStateBase>(GetWorld()->GetGameState());
	}
		//UE_LOG(LogTemp, Warning, TEXT("Exist"));
		WaitingRoomGameStateBase->WholePlayerNumber += 1;
		AWaitingRoomGameModeBase* WaitingRoomGameModeBase = Cast<AWaitingRoomGameModeBase>(GetWorld()->GetAuthGameMode());
		if (WaitingRoomGameStateBase->WholePlayerNumber == WaitingRoomGameModeBase->MaxPlayersNum)
		{
			
			WaitingRoomGameStateBase->MulticastRPC_UpdateNotice("Start Matching");

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, WaitingRoomGameStateBase, &AWaitingRoomGameStateBase::MatchPlayers, 3.0f, false);
		}
	
}
void ARolexPlayerController::ClientRPC_InitWaitingRoomUI_Implementation(const TArray<FString>& IDArray)
{
	TArray<class UPlayerSlotUI*> PlayerSlots = WaitingRoomUI->PlayerSlots;
	for (int32 i = 0; i < IDArray.Num(); i++)
	{
		PlayerSlots[i]->PlayerID->SetText(FText::FromString(IDArray[i]));
	}
}
void ARolexPlayerController::ClientRPC_CreateWaitingRoomUI_Implementation()
{
	if (IsLocalController())
	{
		if (WaitingRoomUIFactory)
		{
			WaitingRoomUI = Cast<UWaitingRoomUI>(CreateWidget(GetWorld(), WaitingRoomUIFactory));
			if (WaitingRoomUI)
				WaitingRoomUI->AddToViewport();
			
			//UE_LOG(LogTemp, Warning, TEXT("Waiting Room Created"));
		}
	}
}
void ARolexPlayerController::ClientRPC_SetPlayerSlotUI_Implementation(int32 PlayerNumber)
{
	// set Steam ID on player slot textbox
	PlayerSlotIndex = PlayerNumber;
	//UE_LOG(LogTemp, Warning, TEXT("Player Controller %s, PlayerSlotIndex %d"), *GetName(), PlayerSlotIndex);
	UPlayerSlotUI* PlayerSlot = WaitingRoomUI->PlayerSlots[PlayerNumber];
	if (PlayerSlot)
	{
		OwnPlayerSlot = PlayerSlot;
		OwnPlayerSlot-> PlayerSlotIndex = PlayerNumber;

		if (WaitingRoomUI->HeroButtonArray.Num() > 0)
		{
			for (UHeroSlotUI* HeroSlotButton: WaitingRoomUI->HeroButtonArray)
			{
					HeroSlotButton->OwnerPlayerSlotIndex = PlayerNumber;
					// UE_LOG(LogTemp, Warning, TEXT("Bind Delegate"));
					// HeroSlotButton->HeroSelectButtonClickedDelegate.BindUObject(
					// 	WaitingRoomGameStateBase, &AWaitingRoomGameStateBase::ServerRPC_SetPlayerHeroImage);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No HeroButtonArray"));
		}
		
		ServerRPC_InformClientPlayerSlotIndex(PlayerNumber, PlayerSlot);
	}
	
		ServerRPC_UpdateWholePlayerNumber();
}


//////////////////////////////////////////////////////////////////////////


void ARolexPlayerController::InitUI()
{
	if (IsLocalController())
	{
		UI_Zone->AddToViewport();
		UI_Zone->UIInit();
	}
}

void ARolexPlayerController::SetPlayTime(float Time)
{
	if (IsLocalController() && UI_Zone)
	{
		UI_Zone->SetPlayTime(Time);
	}
}
void ARolexPlayerController::SetPoint(int idx)
{
	if (IsLocalController() && UI_Zone)
	{
		UI_Zone->SetPoint(idx);
	}
}
void ARolexPlayerController::SetTakingGuage(float Agauge, float Bgauge)
{
	if (IsLocalController() && UI_Zone)
	{
		UI_Zone->SetTakingGuage(Agauge, Bgauge);
	}
}
void ARolexPlayerController::SetATeamCount(int32 Count)
{
	if (IsLocalController() && UI_Zone)
	{
		UI_Zone->SetATeamCount(Count);
	}
}
void ARolexPlayerController::SetBTeamCount(int32 Count)
{
	if (IsLocalController() && UI_Zone)
	{
		UI_Zone->SetBTeamCount(Count);
	}
}
void ARolexPlayerController::SetPercent(EOccupation occupation, float APercent, float BPercent)
{
	if (IsLocalController() && UI_Zone)
	{
		UI_Zone->SetPercent(occupation, APercent, BPercent);
	}
}
void ARolexPlayerController::SetClashing(EClashing clash)
{
	if (IsLocalController() && UI_Zone)
	{
		UI_Zone->SetClashing(clash);
	}
}
void ARolexPlayerController::SetExtraTime(float Time)
{
	if (IsLocalController() && UI_Zone)
	{
		UI_Zone->SetExtraTime(Time);
	}
}
void ARolexPlayerController::SetIsATeamExtraTime(bool bExtra)
{
	if (IsLocalController() and UI_Zone)
	{
		UI_Zone->SetIsATeamExtraTime(bExtra);
	}
}
void ARolexPlayerController::SetIsBTeamExtraTime(bool bExtra)
{
	if (IsLocalController() and UI_Zone)
	{
		UI_Zone->SetIsBTeamExtraTime(bExtra);
	}
}
