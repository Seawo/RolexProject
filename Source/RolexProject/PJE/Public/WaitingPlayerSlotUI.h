// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaitingPlayerSlotUI.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API UWaitingPlayerSlotUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerID;
};
