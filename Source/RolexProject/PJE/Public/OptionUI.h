// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionUI.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API UOptionUI : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(meta = (BindWidget))
	class USlider* AudioVolumeSlider;

	UFUNCTION()
	void OnAudioVolumeChanged(float Value);

	UPROPERTY()
	class AAudioManager* AudioManager;

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* CloseButton;
};


