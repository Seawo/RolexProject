// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/UI/UI_PlayerData.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

#include "RolexPlayerState.h"

void UUI_PlayerData::SetRoleImage(ERoleType Role)
{
	if (Role == ERoleType::Tanker)
	{
		Img_Role->SetBrushFromTexture(RoleImages["Tanker"]);
	}
	else if (Role == ERoleType::Dealer)
	{
		Img_Role->SetBrushFromTexture(RoleImages["Dealer"]);
	}
	else if (Role == ERoleType::Healer)
	{
		Img_Role->SetBrushFromTexture(RoleImages["Healer"]);
	}
}

void UUI_PlayerData::SetIconImage(FName Icon)
{
	Img_Icon->SetBrushFromTexture(IconImages[Icon]);
}

void UUI_PlayerData::SetID(FString ID)
{
	if (ID == "")
	{
		ID = "No ID";
	}
	TB_ID->SetText(FText::FromString(ID));
}

void UUI_PlayerData::SetTeamColor(bool team)
{
	if (team)
	{
		B_Role->SetBrushColor(FLinearColor(0.0f, 0.734375f, 0.689348f, 1.0f));
		B_Icon->SetBrushColor(FLinearColor(0.0f, 0.734375f, 0.689348f, 0.7f));
		B_ID->SetBrushColor(FLinearColor(0.0f, 0.734375f, 0.689348f, 0.5f));
		B_Kill->SetBrushColor(FLinearColor(0.0f, 0.125f, 0.117336f, 0.5f));
		B_Death->SetBrushColor(FLinearColor(0.0f, 0.125f, 0.117336f, 0.5f));
		B_Dealing->SetBrushColor(FLinearColor(0.0f, 0.734375f, 0.689348f, 0.5f));
		B_Healing->SetBrushColor(FLinearColor(0.0f, 0.734375f, 0.689348f, 0.5f));
	}
	else
	{
		B_Role->SetBrushColor(FLinearColor(0.776042f, 0.0f, 0.738465f, 1.0f));
		B_Icon->SetBrushColor(FLinearColor(0.776042f, 0.0f, 0.738465f, 0.7f));
		B_ID->SetBrushColor(FLinearColor(0.776042f, 0.0f, 0.738465f, 0.5f));
		B_Kill->SetBrushColor(FLinearColor(0.276042f, 0.0f, 0.262675f, 0.5f));
		B_Death->SetBrushColor(FLinearColor(0.276042f, 0.0f, 0.262675f, 0.5f));
		B_Dealing->SetBrushColor(FLinearColor(0.776042f, 0.0f, 0.738465f, 0.5f));
		B_Healing->SetBrushColor(FLinearColor(0.776042f, 0.0f, 0.738465f, 0.5f));
	}
}

void UUI_PlayerData::UpdatePlayerData(ARolexPlayerState* ps)
{
	TB_Kill->SetText(FText::FromString(FString::FromInt(ps->PlayerData.KillCount)));
	TB_Death->SetText(FText::FromString(FString::FromInt(ps->PlayerData.DeathCount)));
	TB_Dealing->SetText(FText::FromString(FString::FromInt(ps->PlayerData.Damage)));
	TB_Healing->SetText(FText::FromString(FString::FromInt(ps->PlayerData.Healing)));
}