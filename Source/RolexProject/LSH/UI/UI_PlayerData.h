// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseCharacter.h"
#include "UI_PlayerData.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API UUI_PlayerData : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetRoleImage(ERoleType Role);
	void SetIconImage(FName Icon);
	void SetID(FString ID);

	void SetTeamColor(bool team);

	void UpdatePlayerData(class ARolexPlayerState* ps);
private:
	UPROPERTY(EditAnywhere)
	TMap<FName, class UTexture2D*> RoleImages;

	UPROPERTY(EditAnywhere)
	TMap<FName, class UTexture2D*> IconImages;


	UPROPERTY(meta = (BindWidget))
	class UBorder* B_Role;
	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Role;

	UPROPERTY(meta = (BindWidget))
	class UBorder* B_Icon;
	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Icon;
	

	UPROPERTY(meta = (BindWidget))
	class UBorder* B_ID;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_ID;
	UPROPERTY(meta = (BindWidget))
	class UBorder* B_Kill;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_Kill;
	UPROPERTY(meta = (BindWidget))
	class UBorder* B_Death;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_Death;
	UPROPERTY(meta = (BindWidget))
	class UBorder* B_Dealing;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_Dealing;
	UPROPERTY(meta = (BindWidget))
	class UBorder* B_Healing;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_Healing;


};
