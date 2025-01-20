// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroUI.h"

#include "BaseCharacter.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHeroUI::NativeConstruct()
{
	Super::NativeConstruct();

	// HP is bind in the blueprint
	
	// bind skill and weapon Image
	if (BaseCharacter->RMBSkillImage)
		SkillImage->SetBrushFromTexture(BaseCharacter->RMBSkillImage);
	if (BaseCharacter->ESkillImage)
		SkillImage->SetBrushFromTexture(BaseCharacter->ESkillImage);
	if (BaseCharacter->WeaponImage)
		WeaponImage->SetBrushFromTexture(BaseCharacter->WeaponImage);

	CoolTime = BaseCharacter->Data.ESkillCoolTime;
}
