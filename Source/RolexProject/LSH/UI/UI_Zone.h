// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GM_TrainingRoom.h"
#include "UI_Zone.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API UUI_Zone : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UIInit();

	// Set 함수들
	void SetPlayTime(float Time);						// 플레이 시간 설정

	void SetATeamCount(int32 Count);					// A팀 점령중인 인원 설정
	void SetBTeamCount(int32 Count);					// B팀 점령중인 인원 설정

	void SetTakingGuage(float Agauge, float Bgauge);	// 점령중인 팀의 게이지 설정
	void SetPercent(EOccupation occupation, float APercent, float BPercent);				// 거점 점령중인 퍼센트 설정

	void SetClashing(EClashing clash);			// 점령 상태 설정	 (누가 점령중인지)
	void SetExtraTime(float Time);				// 추가 시간 설정

	void SetIsATeamExtraTime(bool bExtra) { bIsATeamExtraTime = bExtra; }
	void SetIsBTeamExtraTime(bool bExtra) { bIsBTeamExtraTime = bExtra; }

public:
	// SetPlayTime
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TB_PlayTime;

	UPROPERTY(Meta = (BindWidget))
	class UImage* Img_CircleProgressBarA;

	UPROPERTY(Meta = (BindWidget))
	class UImage* Img_CircleProgressBarB;
	UPROPERTY(Meta = (BindWidget))
	class UImage* Img_Circle;


	
	UPROPERTY(Meta = (BindWidget))
	class UImage* Img_Lock;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TB_A;					// index0 거점
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TB_B;					// index1 거점

	// SetATeamPercent
	UPROPERTY(Meta = (BindWidget))
	class UBorder* B_ATeam;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TB_ATeamPercent;

	// SetBTeamPercent
	UPROPERTY(Meta = (BindWidget))
	class UBorder* B_BTeam;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TB_BTeamPercent;


	// SetATeamCount
	UPROPERTY(Meta = (BindWidget))
	class UBorder* B_ATeamCount;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TB_ATeamCount;

	// SetBTeamCount
	UPROPERTY(Meta = (BindWidget))
	class UBorder* B_BTeamCount;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TB_BTeamCount;


	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TB_Clashing;

	UPROPERTY(Meta = (BindWidget))
	class UVerticalBox* VB_ExtraTime;
	UPROPERTY(Meta = (BindWidget))
	class UProgressBar* PB_ExtraTime;

private:
	// Material Instance 가져오기
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	class UMaterialInstanceDynamic* MI_CircleProcessBarA;
	class UMaterialInstanceDynamic* MI_CircleProcessBarB;

	bool bIsATeamExtraTime = false;
	bool bIsBTeamExtraTime = false;
};
