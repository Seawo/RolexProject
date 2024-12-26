// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_InGame.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API UUI_InGame : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_HPBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_RMBCooltime;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_ESkillCooltime;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_QSkillCooltime;
};
