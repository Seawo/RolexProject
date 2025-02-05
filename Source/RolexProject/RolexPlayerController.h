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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UWaitingPlayerSlotUI> WaitingPlayerSlotUIFactory;
	
public:
	class UPlayerSlotUI* OwnPlayerSlot;

public:
	UFUNCTION(Server, Reliable)
	void ServerRPC_UpdateWholePlayerNumber();

	UFUNCTION(Server, Reliable)
	void ServerRPC_InformClientPlayerSlotIndex(int32 PlayerNumber, UPlayerSlotUI* PlayerSlotUI);

public:
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetPlayerSlotHeroImage(UTexture2D* PlayerHeroTexture, int32 Index);

	UFUNCTION(Server, Reliable)
	void ServerRPC_BlockHero(int32 HeroIndex, int32 PlayerIndex);

	UFUNCTION(Server, Reliable)
	void ServerRPC_UnBlockHero(int32 HeroIndex, int32 PlayerIndex);
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetSelectedHero(const FString& ID, TSubclassOf<class ABaseCharacter> BaseCharacter);

	int32 PreviousSelectedHeroSlotIndex;

	
public:
	bool bIsGoInPlayMap;			// 플레이맵으로 넘어가는지 여부
/////////////////////////PlayMap에서 사용될 함수 변수들/////////////////////////

public:
	void InitUI();
	void SetCharacterOverlay(class AGS_TrainingRoom* gs);

public:
	void SetPlayTime(float Time);
	void SetPoint(int idx);
	void SetTakingGuage(float Agauge, float Bgauge);

	void SetResult(EResult result);

	void SetATeamCount(int32 Count);
	void SetBTeamCount(int32 Count);
	void SetPercent(EOccupation occupation, float APercent, float BPercent);

	void SetClashing(EClashing clash);
	void SetExtraTime(float Time);
	void SetOffofTxtraTime();

	void SetIsATeamExtraTime(bool bExtra);
	void SetIsBTeamExtraTime(bool bExtra);

	void OpenInGameEsc();
	void CloseInGameEsc();


	void OpenInGameTab();
	void CloseInGameTab();

	void InitInGameTab();

public:
	class ARolexPlayerState* RolexPS;

	class UMaterialInstanceDynamic* MI_Overlay;
	class UMaterialInstanceDynamic* MI_PostProcess;

	FTimerHandle TimerHandleUI;
	FTimerHandle TimerHandleTabUI;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUI_Zone> UI_ZoneClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	class UUI_Zone* UI_Zone;


	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUI_GameEnd> UI_GameEndClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	class UUI_GameEnd* UI_GameEnd;


	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUI_InGameEsc> UI_InGameEscClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	class UUI_InGameEsc* UI_InGameEsc;


	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUI_InGameTab> UI_InGameTabClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	class UUI_InGameTab* UI_InGameTab;
};
