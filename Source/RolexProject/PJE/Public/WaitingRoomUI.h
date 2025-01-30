// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaitingRoomUI.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API UWaitingRoomUI : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	
public:
	// players box
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* PlayerBox;

	// waiting other players to come in
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* WaitingPlayersBox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class UPlayerSlotUI*> PlayerSlots;

	void GetAllDescendants(UPanelWidget* ParentWidget, TArray<class UHeroSlotUI*>& Descendants);
	
	int32 NumplayerSlots;
	
	void AddPlayer();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RoomName;
	
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WaitingRoomWidgetSwitcher;
	
public:
	// hero selection
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* HeroSelectionPanel;
	
	TArray<UHeroSlotUI*> HeroButtonArray;

	bool HeroSelectionVisible = false;
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Notice;

	UPROPERTY(ReplicatedUsing=OnRep_SetNotice)
	FText NoticeText;

	UFUNCTION()
	void OnRep_SetNotice();
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountDown;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:

	UFUNCTION()
	void TravelToMain();
};
