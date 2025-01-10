// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/UI/UI_Zone.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"


// 거점 안된 색 :  FLinearColor(0.001661f, 0.004162f, 0.015625f, 1.0f)
// A팀 색(파란색) :  FLinearColor(0.0f, 0.646928f, 1.0f , 1.0f)
// B팀 색(빨간색) :  FLinearColor(1.0f, 0.056108f, 0.0f , 1.0f)

void UUI_Zone::UIInit()
{
	TB_A->SetVisibility(ESlateVisibility::Hidden);
	TB_B->SetVisibility(ESlateVisibility::Hidden);


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


}
