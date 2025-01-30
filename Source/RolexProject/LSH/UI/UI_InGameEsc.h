// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_InGameEsc.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API UUI_InGameEsc : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnBtnGamePlayClicked();
	UFUNCTION()
	void OnBtnOptionClicked();
	UFUNCTION()
	void OnBtnGameLeaveClicked();
	UFUNCTION()
	void OnBtnGameOverClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GamePlay;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Option;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GameLeave;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GameOver;
};
