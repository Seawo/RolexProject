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
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* PlayerBox;

	TArray<class UVerticalBox*> PlayerSlots;

	void AddPlayer();
};
