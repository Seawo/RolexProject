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

	//Skill CoolTime Bar
	UPROPERTY(meta = (BindWidget))
	class URadialSlider* QSkillCoolTimeRadialBar;	

	// index order: Q, E, LMB, RMB
	TMap<int32, URadialSlider*> RadialBarMap;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* RMBSkillCoolTimeBar;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ESkillCoolTimeBar;

	// index order: Q, E, LMB, RMB
	TMap<int32, UProgressBar*> ProgressBarMap;

	UPROPERTY()
	FTimerHandle ETimerHandle;

	UPROPERTY()
	FTimerHandle QTimerHandle;
	
	UPROPERTY()
	FTimerHandle RMBTimerHandle;
	
	TMap<int32, FTimerHandle> TimerMap;
	
	float EAccumulateTime = 0.0f;

	float QAccumulateTime = 0.0f;
	
	float RMBAccumulateTime = 0.0f;
	
	TMap<int32, float> AccumulateTimeMap;
	
	// Skill CoolTime
	int32 CoolTime;
	
	void StartCoolTime(int32 SkillIndex, int32 Time);

	void UpdatePercent(int32 SkillIndex, int32 Time);
};

