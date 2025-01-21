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
	{
		FSlateBrush BackGroundBrush;
		BackGroundBrush.SetResourceObject(BaseCharacter->RMBSkillImage);
	
		FProgressBarStyle ProgressBarStyle = RMBSkillCoolTimeBar->GetWidgetStyle();
		ProgressBarStyle.SetBackgroundImage(BackGroundBrush);
		ProgressBarStyle.SetFillImage(BackGroundBrush);
		RMBSkillCoolTimeBar->SetWidgetStyle(ProgressBarStyle);
	}
	
	if (BaseCharacter->ESkillImage)
	{
		FSlateBrush BackGroundBrush;
		BackGroundBrush.SetResourceObject(BaseCharacter->ESkillImage);

		FProgressBarStyle ProgressBarStyle = ESkillCoolTimeBar->GetWidgetStyle();
		ProgressBarStyle.SetBackgroundImage(BackGroundBrush);
		ProgressBarStyle.SetFillImage(BackGroundBrush);
		ESkillCoolTimeBar->SetWidgetStyle(ProgressBarStyle);
	}
	
	if (BaseCharacter->WeaponImage)
		WeaponImage->SetBrushFromTexture(BaseCharacter->WeaponImage);

	CoolTime = BaseCharacter->Data.ESkillCoolTime;
}
