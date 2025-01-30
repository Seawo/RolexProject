// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_GameEnd.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API UUI_GameEnd : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UIInit();
	void SetResultWin();
	void SetResultLose();

private:
	UPROPERTY(Meta = (BindWidget))
	class UBorder* B_BackGround;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TB_Win;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TB_Back_Win;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TB_Fail;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TB_Back_Fail;
};
