// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/UI/UI_GameEnd.h"

#include "Components/TextBlock.h"
#include "Components/Border.h"

void UUI_GameEnd::UIInit()
{
	B_BackGround->SetVisibility(ESlateVisibility::Hidden);
	TB_Win->SetVisibility(ESlateVisibility::Hidden);
	TB_Back_Win->SetVisibility(ESlateVisibility::Hidden);
	TB_Fail->SetVisibility(ESlateVisibility::Hidden);
	TB_Back_Fail->SetVisibility(ESlateVisibility::Hidden);
}

void UUI_GameEnd::SetResultWin()
{
	B_BackGround->SetVisibility(ESlateVisibility::Visible);
	B_BackGround->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.4f));
	TB_Win->SetVisibility(ESlateVisibility::Visible);
	TB_Back_Win->SetVisibility(ESlateVisibility::Visible);
	TB_Fail->SetVisibility(ESlateVisibility::Hidden);
	TB_Back_Fail->SetVisibility(ESlateVisibility::Hidden);
}

void UUI_GameEnd::SetResultLose()
{
	B_BackGround->SetVisibility(ESlateVisibility::Visible);
	B_BackGround->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.25f));
	TB_Win->SetVisibility(ESlateVisibility::Hidden);
	TB_Back_Win->SetVisibility(ESlateVisibility::Hidden);
	TB_Fail->SetVisibility(ESlateVisibility::Visible);
	TB_Back_Fail->SetVisibility(ESlateVisibility::Visible);
}
