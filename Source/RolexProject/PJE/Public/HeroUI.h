// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HeroUI.generated.h"

/**
 *
 */
UCLASS()
class ROLEXPROJECT_API UHeroUI : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ABaseCharacter* BaseCharacter;

	// Skill Image
	UPROPERTY(meta = (BindWidget))
	class UImage* SkillImage;
	
	// Skill CoolTime
	int32 CoolTime;

	// Ammo
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Ammo;
	
	// MaxAmmo
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxAmmo;
	
	// Weapon Image
	UPROPERTY(meta = (BindWidget))
	UImage* WeaponImage;
	
	
};
