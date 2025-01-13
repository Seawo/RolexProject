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

	TArray<class UPlayerSlotUI*> PlayerSlots;

	int32 NumplayerSlots;
	
	void AddPlayer();

public:
	// hero selection
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* HeroSelectionPanel;

	TArray<class UHeroSlotUI*> HeroButtonArray;

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Notice;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountDown;
	
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	UFUNCTION()
	void TravelToMain();
};
