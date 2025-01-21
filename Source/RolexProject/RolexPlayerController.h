// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameState/GS_TrainingRoom.h"
#include "RolexPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API ARolexPlayerController : public APlayerController
{
	GENERATED_BODY()

	ARolexPlayerController();

public:
	virtual void BeginPlay() override;
	
	class AWaitingRoomGameStateBase* WaitingRoomGameStateBase;
		
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UWaitingRoomUI> WaitingRoomUIFactory;

	UWaitingRoomUI* WaitingRoomUI;

	UFUNCTION(Client, Reliable)
	void ClientRPC_CreateWaitingRoomUI();
	
	UFUNCTION(Client, Reliable)
	void ClientRPC_InitWaitingRoomUI(const TArray<FString>& IDArray);

	UFUNCTION(Client, Reliable)
	void ClientRPC_SetPlayerSlotUI(int32 PlayerNumber);

	int32 PlayerSlotIndex;
	
public:
	class UPlayerSlotUI* OwnPlayerSlot;

public:
	UFUNCTION(Server, Reliable)
	void ServerRPC_UpdateWholePlayerNumber();

	UFUNCTION(Server, Reliable)
	void ServerRPC_InformClientPlayerSlotIndex(int32 PlayerNumber, UPlayerSlotUI* PlayerSlotUI);

public:
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetPlayerHeroImage(UTexture2D* PlayerHeroTexture, int32 Index);

	// UFUNCTION(NetMulticast, Reliable)
	// void MulticastRPC_SetPlayerHeroImage(UTexture2D* PlayerHeroTexture, int32 Index);

public:
	// UFUNCTION(Server, Reliable)
	// void ServerRPC_SetSelectedHero(class ARolexPlayerState* RolexPlayerState, TSubclassOf<class ABaseCharacter> BaseCharacter);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetSelectedHero(const FString& ID, TSubclassOf<class ABaseCharacter> BaseCharacter);


	bool bIsGoInPlayMap;			// 플레이맵으로 넘어가는지 여부
/////////////////////////PlayMap에서 사용될 함수 변수들/////////////////////////

public:
	void InitUI();

public:
	void SetPlayTime(float Time);
	void SetPoint(int idx);
	void SetTakingGuage(float Agauge, float Bgauge);

	void SetATeamCount(int32 Count);
	void SetBTeamCount(int32 Count);
	void SetPercent(EOccupation occupation, float APercent, float BPercent);

	void SetClashing(EClashing clash);
	void SetExtraTime(float Time);

	void SetIsATeamExtraTime(bool bExtra);
	void SetIsBTeamExtraTime(bool bExtra);

public:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUI_Zone> UI_ZoneClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	class UUI_Zone* UI_Zone;

	FTimerHandle TimerHandleUI;

};
