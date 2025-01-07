// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionItemUI.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API USessionItemUI : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* SessionJoinBtn;

	UPROPERTY()
	int32 SessionIndex;
	
	UFUNCTION()
	void JoinSession();

	UPROPERTY(meta = (BindWidget))
	FString OwnerName;

	UPROPERTY(meta = (BindWidget))
	FString SessionName;
	
};
