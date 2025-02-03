// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_InGameTab.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API UUI_InGameTab : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:
	void InitData();


	void UpdateData();

private:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUI_PlayerData> PlayerDataFactory;

	UPROPERTY(EditAnywhere)
	TArray<class ABaseCharacter*> ATeam;

	UPROPERTY(EditAnywhere)
	TArray<class ABaseCharacter*> BTeam;


	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* VB_Data;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* VB_ATeam;
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* VB_BTeam;


	bool bOneTime = false;		// 한번만 실행되도록

	class ARolexPlayerState* RolexPS;
};
