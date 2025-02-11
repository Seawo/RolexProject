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
#include "WaitingPlayerSlotUI.h"
#include "Components/VerticalBox.h"
#include "UI_GameEnd.h"
#include "UI_InGameEsc.h"
#include "UI_InGameTab.h"
#include "UI_Loding.h"


#include "Kismet/GameplayStatics.h"


ARolexPlayerController::ARolexPlayerController()
{
}

void ARolexPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController())
	{
		//RolexPS = Cast<ARolexPlayerState>(PlayerState);
	}

	WaitingRoomGameStateBase = Cast<AWaitingRoomGameStateBase>(GetWorld()->GetGameState()); 

	//if (UI_ZoneClass)
	//{
	//	UI_Zone = CreateWidget<UUI_Zone>(this, UI_ZoneClass);
	//	if (UI_Zone)
	//	{
	//		//UI_Zone->AddToViewport();
	//		UE_LOG(LogTemp, Warning, TEXT("UI_Zone is created"));
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("UI_Zone is nullptr"));
	//	}
	//}
}

// 서버 RPC : 플레이어가 선택한 영웅 정보를 서버에 전달
void ARolexPlayerController::ServerRPC_SetSelectedHero_Implementation(const FString& ID,
	const TSubclassOf<class ABaseCharacter> BaseCharacter)
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

void ARolexPlayerController::ServerRPC_SetPlayerSlotHeroImage_Implementation(UTexture2D* PlayerHeroTexture, const int32 Index)
{
	// cannot implement MulticastRPC_SetPlayerHeroImage in RolexPlayerController since clients only have its own controller
	// client GameStateBase cannot call RPC functions since clients doesn't have an ownership for GameStateBase
	WaitingRoomGameStateBase->MulticastRPC_SetPlayerHeroImage(PlayerHeroTexture, Index); 
}

void ARolexPlayerController::ServerRPC_InformClientPlayerSlotIndex_Implementation(const int32 PlayerNumber, UPlayerSlotUI* PlayerSlotUI)
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
		
	WaitingRoomGameStateBase->WholePlayerNumber += 1;
	AWaitingRoomGameModeBase* WaitingRoomGameModeBase = Cast<AWaitingRoomGameModeBase>(GetWorld()->GetAuthGameMode());

	// if the players are all in the room
	if (WaitingRoomGameStateBase->WholePlayerNumber >= WaitingRoomGameModeBase->MaxPlayersNum)
	{
		WaitingRoomGameStateBase->MulticastRPC_UpdateNotice("STARTING SOON ...");
		
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, WaitingRoomGameStateBase, &AWaitingRoomGameStateBase::MulticastRPC_StartHeroSelection, 3.0f, false);
	}

	// in MulticastRPC_StartHeroSelection_Implementation - WaitingRoomGameStateBase 
	// WaitingRoomGameStateBase->MulticastRPC_UpdateNotice("START MATCHING");
	//
	// FTimerHandle TimerHandle;
	// GetWorld()->GetTimerManager().SetTimer(TimerHandle, WaitingRoomGameStateBase, &AWaitingRoomGameStateBase::MatchPlayers, 3.0f, false);
}

// init existing players information for the new player
void ARolexPlayerController::ClientRPC_InitWaitingRoomUI_Implementation(const TArray<FString>& IDArray)
{
	TArray<UPlayerSlotUI*> PlayerSlots = WaitingRoomUI->PlayerSlots;
	for (int32 i = 0; i < IDArray.Num(); i++)
	{
		UWaitingPlayerSlotUI* WaitingPlayerSlotUI = Cast<UWaitingPlayerSlotUI>(CreateWidget(GetWorld(), WaitingPlayerSlotUIFactory));
		if (WaitingPlayerSlotUI)
		{
			WaitingPlayerSlotUI->PlayerID->SetText(FText::FromString(IDArray[i]));
			WaitingRoomUI->WaitingPlayersBox->AddChild(WaitingPlayerSlotUI);
		}
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
		}
	}
}

void ARolexPlayerController::ClientRPC_SetPlayerSlotUI_Implementation(const int32 PlayerNumber)
{
	// set Steam ID on player slot textbox
	PlayerSlotIndex = PlayerNumber;
	
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

	// notify current number of players to the server
	ServerRPC_UpdateWholePlayerNumber();
}

void ARolexPlayerController::ServerRPC_BlockHero_Implementation(const int32 HeroIndex, const int32 PlayerIndex)
{
	WaitingRoomGameStateBase->MulticastRPC_BlockHero(HeroIndex, PlayerIndex);
}

void ARolexPlayerController::ServerRPC_UnBlockHero_Implementation(const int32 HeroIndex, const int32 PlayerIndex)
{
	WaitingRoomGameStateBase->MulticastRPC_UnBlockHero(HeroIndex, PlayerIndex);
}

//////////////////////////////////////////////////////////////////////////

void ARolexPlayerController::ServerRPC_SetText_Implementation(const FString& String)
{
	WaitingRoomGameStateBase->MulticastRPC_SetText(String);
}

void ARolexPlayerController::InitUI()
{
	if (not IsLocalController()) return;


	GetWorld()->GetTimerManager().SetTimer(TimerHandleUI, [this]()
	{
			if (UI_ZoneClass)
			{
				UI_Zone = CreateWidget<UUI_Zone>(this, UI_ZoneClass);
				if (UI_Zone)
				{
					UI_Zone->AddToViewport();
					UI_Zone->UIInit();
					UE_LOG(LogTemp, Warning, TEXT("UI_Zone is created"));
					GetWorld()->GetTimerManager().ClearTimer(TimerHandleUI);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("UI_Zone is nullptr"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("UI_ZoneClass is nullptr"));
			}
	}, 0.1f, true);

	if (UI_InGameTabClass)
	{
		UI_InGameTab = CreateWidget<UUI_InGameTab>(this, UI_InGameTabClass);
		if (UI_InGameTab)
		{
			UE_LOG(LogTemp, Warning, TEXT("UI_InGameTab is created"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UI_InGameTab is nullptr"));
		}
	}



	//if (IsLocalController() && UI_ZoneClass)
	//{
	//	UI_Zone = CreateWidget<UUI_Zone>(this, UI_ZoneClass);
	//
	//	if (UI_Zone)
	//	{
	//		UI_Zone->AddToViewport();
	//		UI_Zone->UIInit();
	//
	//		UE_LOG(LogTemp, Warning, TEXT("[RolexPlayerController InitUI] UI_Zone is created"));
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("[RolexPlayerController InitUI] UI_Zone is nullptr"));
	//	}
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("[RolexPlayerController InitUI] UI_ZoneClass is nullptr"));
	//}
}

void ARolexPlayerController::SetCharacterOverlay(class AGS_TrainingRoom* gs)
{
	if (IsLocalController())
	{
		UE_LOG(LogTemp, Warning, TEXT("SetCharacterOverlay() 실행됨! - %p, IsLocalController: %d"), this, IsLocalController());

		//ABaseCharacter* owner = Cast<ABaseCharacter>(GetPawn());
		/*
		for (ABaseCharacter* character : gs->ATeamChracters)
		{
			if (character == owner) continue;
			
			// 다른팀인경우
			if (character->Data.Team != owner->Data.Team)
			{
				UMaterialInterface* material = Cast<UMaterialInterface>(character->GetMesh()->GetOverlayMaterial());

				if (material)
				{
					MI_Overlay = UMaterialInstanceDynamic::Create(material, this);
					character->GetMesh()->SetOverlayMaterial(MI_Overlay);
					// Overlay Material의 색을 빨간색으로 설정
					MI_Overlay->SetVectorParameterValue("Color", FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
				}

				character->GetMesh()->SetCustomDepthStencilValue(2);
				//baseCharacter->GetMesh()->MarkRenderStateDirty();
			}
			// 같은 팀인 경우
			else
			{
				UMaterialInterface* material = Cast<UMaterialInterface>(character->GetMesh()->GetOverlayMaterial());

				if (material)
				{
					MI_Overlay = UMaterialInstanceDynamic::Create(material, this);
					character->GetMesh()->SetOverlayMaterial(MI_Overlay);
					// Overlay Material의 색을 청색으로 설정
					MI_Overlay->SetVectorParameterValue("Color", FLinearColor(0.0f, 0.859375f, 0.250501f, 1.0f));
				}

				//AGS_TrainingRoom* GS = Cast<AGS_TrainingRoom>(GetWorld()->GetGameState());
				if (gs)
				{
					UMaterialInterface* ppmaterial = Cast<UMaterialInterface>(gs->MaterialInstance);

					if (ppmaterial)
					{
						MI_PostProcess = UMaterialInstanceDynamic::Create(ppmaterial, this);
						gs->PostProcessVolume->AddOrUpdateBlendable(MI_PostProcess, 1.0f);
						// PostProcess Material의 색을 청색으로 설정

					}
				}


				if (owner->Data.RoleType == ERoleType::Healer)
				{
					character->GetMesh()->SetRenderCustomDepth(true);
					character->GetMesh()->SetCustomDepthStencilValue(1);

					MI_PostProcess->SetScalarParameterValue("Scale", 0.0f);
				}
				else
				{
					MI_PostProcess->SetScalarParameterValue("Scale", 0.2f);
				}

				//baseCharacter->GetMesh()->MarkRenderStateDirty();
			}
		}
		for (ABaseCharacter* character : gs->BTeamChracters)
		{
			if (character == owner) continue;

			// 다른팀인경우
			if (character->Data.Team != owner->Data.Team)
			{
				UMaterialInterface* material = Cast<UMaterialInterface>(character->GetMesh()->GetOverlayMaterial());

				if (material)
				{
					MI_Overlay = UMaterialInstanceDynamic::Create(material, this);
					character->GetMesh()->SetOverlayMaterial(MI_Overlay);
					// Overlay Material의 색을 빨간색으로 설정
					MI_Overlay->SetVectorParameterValue("Color", FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
				}

				character->GetMesh()->SetCustomDepthStencilValue(2);
				//baseCharacter->GetMesh()->MarkRenderStateDirty();
			}
			// 같은 팀인 경우
			else
			{
				UMaterialInterface* material = Cast<UMaterialInterface>(character->GetMesh()->GetOverlayMaterial());

				if (material)
				{
					MI_Overlay = UMaterialInstanceDynamic::Create(material, this);
					character->GetMesh()->SetOverlayMaterial(MI_Overlay);
					// Overlay Material의 색을 청색으로 설정
					MI_Overlay->SetVectorParameterValue("Color", FLinearColor(0.0f, 0.859375f, 0.250501f, 1.0f));
				}

				//AGS_TrainingRoom* GS = Cast<AGS_TrainingRoom>(GetWorld()->GetGameState());
				if (gs)
				{
					UMaterialInterface* ppmaterial = Cast<UMaterialInterface>(gs->MaterialInstance);

					if (ppmaterial)
					{
						MI_PostProcess = UMaterialInstanceDynamic::Create(ppmaterial, this);
						gs->PostProcessVolume->AddOrUpdateBlendable(MI_PostProcess, 1.0f);
						// PostProcess Material의 색을 청색으로 설정

					}
				}


				if (owner->Data.RoleType == ERoleType::Healer)
				{
					character->GetMesh()->SetRenderCustomDepth(true);
					character->GetMesh()->SetCustomDepthStencilValue(1);

					MI_PostProcess->SetScalarParameterValue("Scale", 0.0f);
				}
				else
				{
					MI_PostProcess->SetScalarParameterValue("Scale", 0.2f);
				}

				//baseCharacter->GetMesh()->MarkRenderStateDirty();
			}
		}
		*/

		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), foundActors);
		UE_LOG(LogTemp, Warning, TEXT("foundActors Num : %d"), foundActors.Num());
		for (AActor* actor : foundActors)
		{
			ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(actor);
			ABaseCharacter* owner = Cast<ABaseCharacter>(GetPawn());
			
			if (baseCharacter == owner) continue;
		
			// 다른팀인경우
			if (baseCharacter->Data.Team != owner->Data.Team)
			{
				UMaterialInterface* material = Cast<UMaterialInterface>(baseCharacter->GetMesh()->GetOverlayMaterial());
		
				if (material)
				{
					MI_Overlay = UMaterialInstanceDynamic::Create(material, this);
					baseCharacter->GetMesh()->SetOverlayMaterial(MI_Overlay);
					// Overlay Material의 색을 빨간색으로 설정
					MI_Overlay->SetVectorParameterValue("Color", FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
				}
		
				baseCharacter->GetMesh()->SetCustomDepthStencilValue(2);
				//baseCharacter->GetMesh()->MarkRenderStateDirty();
			}
			// 같은 팀인 경우
			else
			{
				UMaterialInterface* material = Cast<UMaterialInterface>(baseCharacter->GetMesh()->GetOverlayMaterial());
		
				if (material)
				{
					MI_Overlay = UMaterialInstanceDynamic::Create(material, this);
					baseCharacter->GetMesh()->SetOverlayMaterial(MI_Overlay);
					// Overlay Material의 색을 청색으로 설정
					MI_Overlay->SetVectorParameterValue("Color", FLinearColor(0.0f, 0.859375f, 0.250501f, 1.0f));
				}
				
				//AGS_TrainingRoom* GS = Cast<AGS_TrainingRoom>(GetWorld()->GetGameState());
				if (gs)
				{
					UMaterialInterface* ppmaterial = Cast<UMaterialInterface>(gs->MaterialInstance);
		
					if (ppmaterial)
					{
						MI_PostProcess = UMaterialInstanceDynamic::Create(ppmaterial, this);
						gs->PostProcessVolume->AddOrUpdateBlendable(MI_PostProcess, 1.0f);
						// PostProcess Material의 색을 청색으로 설정
						
					}
				}
		
		
				if (owner->Data.RoleType == ERoleType::Healer)
				{
					baseCharacter->GetMesh()->SetRenderCustomDepth(true);
					baseCharacter->GetMesh()->SetCustomDepthStencilValue(1);
		
					MI_PostProcess->SetScalarParameterValue("Scale", 0.0f);
				}
				else
				{
					MI_PostProcess->SetScalarParameterValue("Scale", 0.2f);
				}
		
				//baseCharacter->GetMesh()->MarkRenderStateDirty();
			}
		
		
		}
	}
}

void ARolexPlayerController::AddLodingUI()
{
	if (IsLocalController() && UI_LodingClass)
	{
		UI_Loding = CreateWidget<UUI_Loding>(this, UI_LodingClass);
		if (UI_Loding)
		{
			UI_Loding->AddToViewport();
		}
	}
}

void ARolexPlayerController::RemoveLodingUI()
{
	if (IsLocalController() && UI_Loding)
	{
		UI_Loding->RemoveFromParent();
	}
}

void ARolexPlayerController::SetPlayTime(const float Time)
{
	if (IsLocalController() && UI_Zone)
	{
		UI_Zone->SetPlayTime(Time);
	}
}
void ARolexPlayerController::SetPoint(const int idx)
{
	if (IsLocalController() && UI_Zone)
	{
		UI_Zone->SetPoint(idx);
	}
}
void ARolexPlayerController::SetTakingGuage(const float Agauge, const float Bgauge)
{
	if (IsLocalController() && UI_Zone)
	{
		float A = (int)Agauge / 100.0f;
		float B = (int)Bgauge / 100.0f;
		UI_Zone->SetTakingGuage(A, B);
	}
}
void ARolexPlayerController::SetResult(const EResult result)
{
	if (not IsLocalController()) return;

	if (UI_GameEndClass)
	{
		UI_GameEnd = CreateWidget<UUI_GameEnd>(this, UI_GameEndClass);
		UE_LOG(LogTemp, Warning, TEXT("UI_GameEnd is created"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UI_GameEndClass is nullptr"));
	}

	if (UI_GameEnd)
	{
		UE_LOG(LogTemp, Warning, TEXT("UI_GameEnd is not nullptr"));
		UI_GameEnd->AddToViewport();
		//UI_GameEnd->UIInit();

		ABaseCharacter* character = Cast<ABaseCharacter>(GetPawn());
		AGS_TrainingRoom* GS = Cast<AGS_TrainingRoom>(GetWorld()->GetGameState());


		if (character)
		{
			UE_LOG(LogTemp, Warning, TEXT("Character is %s"), *character->GetName());
		}

		if (result == EResult::AWin)
		{
			if (character and character->Data.Team)
			{
				UI_GameEnd->SetResultWin();
				if (GS)
				{
					GS->GameSoundPlay("Victory");
				}
			}
			else if (character and not character->Data.Team)
			{
				UI_GameEnd->SetResultLose();

				if (GS)
				{
					GS->GameSoundPlay("Defeat");
				}
			}
		}
		else if (result == EResult::BWin)
		{
			if (character and character->Data.Team)
			{
				UI_GameEnd->SetResultLose();

				if (GS)
				{
					GS->GameSoundPlay("Defeat");
				}
			}
			else if (character and not character->Data.Team)
			{
				UI_GameEnd->SetResultWin();

				if (GS)
				{
					GS->GameSoundPlay("Victory");
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UI_GameEnd is nullptr"));
	}
}
void ARolexPlayerController::SetATeamCount(const int32 Count)
{
	if (IsLocalController() && UI_Zone)
	{
		UI_Zone->SetATeamCount(Count);
	}
}
void ARolexPlayerController::SetBTeamCount(const int32 Count)
{
	if (IsLocalController() && UI_Zone)
	{
		UI_Zone->SetBTeamCount(Count);
	}
}
void ARolexPlayerController::SetPercent(const EOccupation occupation, const float APercent, const float BPercent)
{
	if (IsLocalController() && UI_Zone)
	{
		UI_Zone->SetPercent(occupation, APercent, BPercent);
	}
}
void ARolexPlayerController::SetClashing(const EClashing clash)
{
	if (IsLocalController() && UI_Zone)
	{
		UI_Zone->SetClashing(clash);
	}
}
void ARolexPlayerController::SetExtraTime(const float Time)
{
	if (IsLocalController() && UI_Zone)
	{
		UI_Zone->SetExtraTime(Time);
	}
}
void ARolexPlayerController::SetOffofTxtraTime()
{
	if (IsLocalController() && UI_Zone)
	{
		UI_Zone->SetOffofTxtraTime();
	}
}
void ARolexPlayerController::SetIsATeamExtraTime(const bool bExtra)
{
	if (IsLocalController() and UI_Zone)
	{
		UI_Zone->SetIsATeamExtraTime(bExtra);
	}
}
void ARolexPlayerController::SetIsBTeamExtraTime(const bool bExtra)
{
	if (IsLocalController() and UI_Zone)
	{
		UI_Zone->SetIsBTeamExtraTime(bExtra);
	}
}

void ARolexPlayerController::OpenInGameEsc()
{
	if (IsLocalController() && UI_InGameEscClass)
	{
		if (UI_InGameEsc)
		{
			UI_InGameEsc->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UI_InGameEsc is nullptr"));
			UI_InGameEsc = CreateWidget<UUI_InGameEsc>(this, UI_InGameEscClass);
			UE_LOG(LogTemp, Warning, TEXT("UI_InGameEsc Create"));
			if (UI_InGameEsc)
			{
				UI_InGameEsc->AddToViewport();
			}
		}
	}
}
void ARolexPlayerController::CloseInGameEsc()
{
	if (IsLocalController() && UI_InGameEsc)
	{
		UI_InGameEsc->RemoveFromParent();
	}
}

void ARolexPlayerController::OpenInGameTab()
{
	if (IsLocalController() && UI_InGameTab)
	{
		UI_InGameTab->AddToViewport();
		// 플레이어가 Tab키를 누를때마다 데이터 업데이트

		GetWorld()->GetTimerManager().SetTimer(TimerHandleTabUI, [this]()
		{
			UI_InGameTab->UpdateData();
		}, 0.1f, true);
	}
}
void ARolexPlayerController::CloseInGameTab()
{
	if (IsLocalController() && UI_InGameTab)
	{
		UI_InGameTab->RemoveFromParent();
		GetWorld()->GetTimerManager().ClearTimer(TimerHandleTabUI);
	}
}
void ARolexPlayerController::InitInGameTab()
{
	if (IsLocalController() && UI_InGameTab)
	{
		UI_InGameTab->InitData();
	}
}