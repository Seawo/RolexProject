// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/UI/UI_Zone.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/ProgressBar.h"


// 거점 안된 색 :  FLinearColor(0.001661f, 0.004162f, 0.015625f, 1.0f)
// A팀 색(파란색) :  FLinearColor(0.0f, 0.646928f, 1.0f , 1.0f)
// B팀 색(빨간색) :  FLinearColor(1.0f, 0.056108f, 0.0f , 1.0f)

void UUI_Zone::UIInit()
{
	TB_A->SetVisibility(ESlateVisibility::Hidden);
	TB_B->SetVisibility(ESlateVisibility::Hidden);

	Img_CircleProgressBarA->SetVisibility(ESlateVisibility::Hidden);
	Img_CircleProgressBarB->SetVisibility(ESlateVisibility::Hidden);

	B_ATeam->SetBrushColor(FLinearColor(0.001661f, 0.004162f, 0.015625f, 1.0f));
	B_BTeam->SetBrushColor(FLinearColor(0.001661f, 0.004162f, 0.015625f, 1.0f));

	TB_ATeamPercent->SetText(FText::FromString("0"));
	TB_BTeamPercent->SetText(FText::FromString("0"));

	TB_ATeamCount->SetText(FText::FromString("0"));
	TB_BTeamCount->SetText(FText::FromString("0"));

	B_ATeamCount->SetBrushColor(FLinearColor(0.0f, 0.646928f, 1.0f, 1.0f));
	B_ATeamCount->SetVisibility(ESlateVisibility::Hidden);
	B_BTeamCount->SetBrushColor(FLinearColor(1.0f, 0.056108f, 0.0f, 1.0f));
	B_BTeamCount->SetVisibility(ESlateVisibility::Hidden);

	TB_Clashing->SetVisibility(ESlateVisibility::Hidden);
	VB_ExtraTime->SetVisibility(ESlateVisibility::Hidden);
}


void UUI_Zone::SetPlayTime(float Time)
{
	TB_PlayTime->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)Time)));
}

void UUI_Zone::SetPoint(int idx)
{
	if (idx == 0)
	{
		Img_Lock->SetVisibility(ESlateVisibility::Hidden);
		TB_A->SetVisibility(ESlateVisibility::Visible);
		TB_B->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (idx == 1)
	{
		Img_Lock->SetVisibility(ESlateVisibility::Hidden);
		TB_A->SetVisibility(ESlateVisibility::Hidden);
		TB_B->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUI_Zone::SetATeamCount(int32 Count)
{
	if (Count == 0)
	{
				B_ATeamCount->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		B_ATeamCount->SetVisibility(ESlateVisibility::Visible);
		TB_ATeamCount->SetText(FText::FromString(FString::FromInt(Count)));
	}
}

void UUI_Zone::SetBTeamCount(int32 Count)
{
	if (Count == 0)
	{
		B_BTeamCount->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		B_BTeamCount->SetVisibility(ESlateVisibility::Visible);
		TB_BTeamCount->SetText(FText::FromString(FString::FromInt(Count)));
	}
}

void UUI_Zone::SetTakingGuage(float Agauge, float Bgauge)
{
	//UE_LOG(LogTemp, Log, TEXT("A : %f, B : %f"), Agauge, Bgauge);

	if (Bgauge >= 0.97 or Agauge >= 0.97 or (Agauge <= 0.01f and Bgauge <= 0.01f))
	{
		Img_CircleProgressBarA->SetVisibility(ESlateVisibility::Hidden);
		Img_CircleProgressBarB->SetVisibility(ESlateVisibility::Hidden);
	}


	else if (Bgauge > 0.01 and Agauge <= 0.01)
	{
		Img_CircleProgressBarA->SetVisibility(ESlateVisibility::Hidden);
		Img_CircleProgressBarB->SetVisibility(ESlateVisibility::Visible);

		UMaterialInterface* materialInterface = Cast<UMaterialInterface>(Img_CircleProgressBarB->Brush.GetResourceObject());
		if (materialInterface)
		{
			if (not MI_CircleProcessBarB)
			{
				MI_CircleProcessBarB = UMaterialInstanceDynamic::Create(materialInterface, this);
				Img_CircleProgressBarB->SetBrushFromMaterial(MI_CircleProcessBarB);
			}
			MI_CircleProcessBarB->SetScalarParameterValue(FName("Percent"), Bgauge);
		}
	}

	else if (Agauge > 0.01 and Bgauge <= 0.01)
	{
		Img_CircleProgressBarA->SetVisibility(ESlateVisibility::Visible);
		Img_CircleProgressBarB->SetVisibility(ESlateVisibility::Hidden);

		UMaterialInterface* materialInterface = Cast<UMaterialInterface>(Img_CircleProgressBarA->Brush.GetResourceObject());
		if (materialInterface)
		{
			if (not MI_CircleProcessBarA)
			{
				MI_CircleProcessBarA = UMaterialInstanceDynamic::Create(materialInterface, this);
				Img_CircleProgressBarA->SetBrushFromMaterial(MI_CircleProcessBarA);
			}
			MI_CircleProcessBarA->SetScalarParameterValue(FName("Percent"), Agauge);
		}
	}
}
void UUI_Zone::SetPercent(EOccupation occupation, float APercent, float BPercent)
{
	if (occupation == EOccupation::TeamA)
	{
		B_ATeam->SetBrushColor(FLinearColor(0.0f, 0.646928f, 1.0f, 1.0f));
		B_BTeam->SetBrushColor(FLinearColor(0.001661f, 0.004162f, 0.015625f, 1.0f));
		TB_ATeamPercent->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)APercent)));
		//UE_LOG(LogTemp, Log, TEXT("A : %d"), FCString::Atoi(*TB_ATeamPercent->GetText().ToString()));
	}
	else if (occupation == EOccupation::TeamB)
	{
		B_ATeam->SetBrushColor(FLinearColor(0.001661f, 0.004162f, 0.015625f, 1.0f));
		B_BTeam->SetBrushColor(FLinearColor(1.0f, 0.056108f, 0.0f, 1.0f));
		TB_BTeamPercent->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)BPercent)));
	}

}

void UUI_Zone::SetClashing(EClashing clash)
{
	if (clash == EClashing::Clash)
	{
		TB_Clashing->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		TB_Clashing->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUI_Zone::SetExtraTime(float Time)
{
	if ((bIsATeamExtraTime and FCString::Atoi(*TB_BTeamPercent->GetText().ToString()) >= 99) or (bIsBTeamExtraTime and FCString::Atoi(*TB_ATeamPercent->GetText().ToString()) >= 99))
	{
		VB_ExtraTime->SetVisibility(ESlateVisibility::Visible);

		float percent = Time / 8.0f;

		PB_ExtraTime->SetPercent(percent);
	}
	else
	{
		VB_ExtraTime->SetVisibility(ESlateVisibility::Hidden);
	}
}
