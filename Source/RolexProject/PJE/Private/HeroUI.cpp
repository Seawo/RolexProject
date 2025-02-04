// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroUI.h"

#include "BaseCharacter.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/RadialSlider.h"
#include "Components/TextBlock.h"
#include "Components/TimelineComponent.h"

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
		// set background image
		ProgressBarStyle.SetBackgroundImage(BackGroundBrush);
		ProgressBarStyle.BackgroundImage.TintColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.2f);
		// set image
		ProgressBarStyle.SetFillImage(BackGroundBrush);
		ProgressBarStyle.FillImage.TintColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
		
		RMBSkillCoolTimeBar->SetWidgetStyle(ProgressBarStyle);
	}
	
	if (BaseCharacter->ESkillImage)
	{
		FSlateBrush BackGroundBrush;
		BackGroundBrush.SetResourceObject(BaseCharacter->ESkillImage);

		FProgressBarStyle ProgressBarStyle = ESkillCoolTimeBar->GetWidgetStyle();
		// set background image
		ProgressBarStyle.SetBackgroundImage(BackGroundBrush);
		ProgressBarStyle.BackgroundImage.TintColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.2f);
		// set image
		ProgressBarStyle.SetFillImage(BackGroundBrush);
		ProgressBarStyle.FillImage.TintColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
		
		ESkillCoolTimeBar->SetWidgetStyle(ProgressBarStyle);
	}

	CoolTime = BaseCharacter->Data.ESkillCoolTime;

	ProgressBarMap=
	   {
		{1, ESkillCoolTimeBar},
		{3, RMBSkillCoolTimeBar}
	   };

	RadialBarMap =
		{
		{0, QSkillCoolTimeRadialBar}
		};
	
	TimerMap =
		{
		{0, QTimerHandle},
		{1, ETimerHandle},
		{3, RMBTimerHandle}
		};

	AccumulateTimeMap =
		{
		{0, QAccumulateTime},
		{1, EAccumulateTime},
		{3, RMBAccumulateTime}
		};
}

void UHeroUI::StartCoolTime(int32 SkillIndex, int32 Time)
{
	// index order: Q, E, LMB, RMB
	AccumulateTimeMap[SkillIndex] = 0.0f;
	if (SkillIndex == 0)
	{
		RadialBarMap[SkillIndex]->SetValue(AccumulateTimeMap[SkillIndex]/Time);
	}
	else
	{
		ProgressBarMap[SkillIndex]->SetPercent(AccumulateTimeMap[SkillIndex]/Time);
	}

	// update percentage by using timer
	GetWorld()->GetTimerManager().SetTimer(
		TimerMap[SkillIndex],
		FTimerDelegate::CreateLambda(
			[this, SkillIndex, Time]()
			{
				UpdatePercent(SkillIndex, Time);
			}),
		0.1,
		true);
}

void UHeroUI::UpdatePercent(int32 SkillIndex, int32 Time)
{
	// if the cool time is charged, clear the timer
	if (AccumulateTimeMap[SkillIndex] >= Time)
	{
		AccumulateTimeMap[SkillIndex] = 0.0f;
		GetWorld()->GetTimerManager().ClearTimer(TimerMap[SkillIndex]);
		return;
	}
	
	AccumulateTimeMap[SkillIndex] += 0.1f;

	// set percent on progress bar
	if (SkillIndex == 0)
	{
		RadialBarMap[SkillIndex]->SetValue(AccumulateTimeMap[SkillIndex]/Time);
	}
	else
	{
		ProgressBarMap[SkillIndex]->SetPercent(AccumulateTimeMap[SkillIndex]/Time);
	}
}
