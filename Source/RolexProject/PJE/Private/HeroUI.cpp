// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroUI.h"

#include "BaseCharacter.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHeroUI::NativeConstruct()
{
	Super::NativeConstruct();

	// HP
	HpString = FString::FromInt(BaseCharacter->Data.Hp);
	MaxHpString = FString::FromInt(BaseCharacter->Data.MaxHp);
	HpPercent = BaseCharacter->Data.Hp/BaseCharacter->Data.MaxHp;
	
	Hp->SetText(FText::FromString(HpString));
	MaxHp->SetText(FText::FromString(MaxHpString));
	HpBar->SetPercent(HpPercent);
	
	//
	SkillImage->SetBrushFromTexture(BaseCharacter->SkillImage);

	CoolTime = BaseCharacter->Data.ESkillCoolTime;

	WeaponImage->SetBrushFromTexture(BaseCharacter->WeaponImage);
}
