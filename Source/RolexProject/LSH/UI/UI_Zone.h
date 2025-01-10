// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Zone.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API UUI_Zone : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UIInit();


public:
	UPROPERTY(Meta = (BindWidget))
	class UImage* Img_CircleProgressBar;
	UPROPERTY(Meta = (BindWidget))
	class UImage* Img_Circle;


	UPROPERTY(Meta = (BindWidget))
	class UImage* Img_Lock;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TB_A;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TB_B;

	UPROPERTY(Meta = (BindWidget))
	class UBorder* B_ATeam;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TB_ATeamPercent;

	UPROPERTY(Meta = (BindWidget))
	class UBorder* B_BTeam;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TB_BTeamPercent;


	UPROPERTY(Meta = (BindWidget))
	class UBorder* B_ATeamCount;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TB_ATeamCount;

	UPROPERTY(Meta = (BindWidget))
	class UBorder* B_BTeamCount;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TB_BTeamCount;

};
