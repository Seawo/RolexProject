// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_InGameTab.h"
#include "UI_PlayerData.h"
#include "Components/VerticalBox.h"

#include "Kismet/GameplayStatics.h"
#include "BaseCharacter.h"
#include "RolexPlayerState.h"
#include "../GameState/GS_TrainingRoom.h"
void UUI_InGameTab::NativeConstruct()
{
	Super::NativeConstruct();

	
	if (not bOneTime)
	{
		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), foundActors);
		UE_LOG(LogTemp, Warning, TEXT("foundActors Num : %d"), foundActors.Num());
		for (AActor* actor : foundActors)
		{
			ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(actor);
			if (baseCharacter)
			{
				if (baseCharacter->Data.Team)
				{
					ATeam.Add(baseCharacter);
				}
				else
				{
					BTeam.Add(baseCharacter);
				}
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("ATeam : %d, BTeam : %d"), ATeam.Num(), BTeam.Num());



		for (ABaseCharacter* character : ATeam)
		{
			UUI_PlayerData* PlayerData = CreateWidget<UUI_PlayerData>(GetWorld(), PlayerDataFactory);

			ARolexPlayerState* playerState = Cast<ARolexPlayerState>(character->GetPlayerState());

			//PlayerData->SetID(playerState->UniqueID);
			PlayerData->SetID(character->Data.Name.ToString());
			PlayerData->SetIconImage(character->Data.Name);
			PlayerData->SetRoleImage(character->Data.RoleType);
			PlayerData->SetTeamColor(true);

			VB_ATeam->AddChild(PlayerData);

		}

		for (ABaseCharacter* character : BTeam)
		{
			UUI_PlayerData* PlayerData = CreateWidget<UUI_PlayerData>(GetWorld(), PlayerDataFactory);

			ARolexPlayerState* playerState = Cast<ARolexPlayerState>(character->GetPlayerState());

			//PlayerData->SetID(playerState->UniqueID);
			PlayerData->SetID(character->Data.Name.ToString());
			PlayerData->SetIconImage(character->Data.Name);
			PlayerData->SetRoleImage(character->Data.RoleType);
			PlayerData->SetTeamColor(false);

			VB_BTeam->AddChild(PlayerData);
		}

		bOneTime = true;
	}
	



	//VB_BTeam->AddChild(PlayerData);

	//VB_Data->AddChild(PlayerData);


}

void UUI_InGameTab::InitData()
{
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), foundActors);
	UE_LOG(LogTemp, Warning, TEXT("foundActors Num : %d"), foundActors.Num());
	for (AActor* actor : foundActors)
	{
		ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(actor);
		if (baseCharacter)
		{
			if (baseCharacter->Data.Team)
			{
				ATeam.Add(baseCharacter);
			}
			else
			{
				BTeam.Add(baseCharacter);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("ATeam : %d, BTeam : %d"), ATeam.Num(), BTeam.Num());



	for (ABaseCharacter* character : ATeam)
	{
		UUI_PlayerData* PlayerData = CreateWidget<UUI_PlayerData>(GetWorld(), PlayerDataFactory);

		ARolexPlayerState* playerState = Cast<ARolexPlayerState>(character->GetPlayerState());

		//PlayerData->SetID(playerState->UniqueID);
		PlayerData->SetID(character->Data.Name.ToString());
		PlayerData->SetIconImage(character->Data.Name);
		PlayerData->SetRoleImage(character->Data.RoleType);
		PlayerData->SetTeamColor(true);

		VB_ATeam->AddChild(PlayerData);

	}

	for (ABaseCharacter* character : BTeam)
	{
		UUI_PlayerData* PlayerData = CreateWidget<UUI_PlayerData>(GetWorld(), PlayerDataFactory);

		ARolexPlayerState* playerState = Cast<ARolexPlayerState>(character->GetPlayerState());

		//PlayerData->SetID(playerState->UniqueID);
		PlayerData->SetID(character->Data.Name.ToString());
		PlayerData->SetIconImage(character->Data.Name);
		PlayerData->SetRoleImage(character->Data.RoleType);
		PlayerData->SetTeamColor(false);

		VB_BTeam->AddChild(PlayerData);
	}
}

void UUI_InGameTab::UpdateData()
{
	for (int32 i = 0; i < ATeam.Num(); i++)
	{
		ABaseCharacter* character = ATeam[i];
		UUI_PlayerData* PlayerData = Cast<UUI_PlayerData>(VB_ATeam->GetChildAt(i));
		ARolexPlayerState* playerState = Cast<ARolexPlayerState>(character->GetPlayerState());
		if (playerState)
		{
			PlayerData->UpdatePlayerData(playerState);
			//UE_LOG(LogTemp, Warning, TEXT("ATeam PlayerData Update"));
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("ATeam playerState is nullptr"));
		}
	}

	for (int32 i = 0; i < BTeam.Num(); i++)
	{
		ABaseCharacter* character = BTeam[i];
		UUI_PlayerData* PlayerData = Cast<UUI_PlayerData>(VB_BTeam->GetChildAt(i));
		ARolexPlayerState* playerState = Cast<ARolexPlayerState>(character->GetPlayerState());
		if (playerState)
		{
			PlayerData->UpdatePlayerData(playerState);
			//UE_LOG(LogTemp, Warning, TEXT("BTeam PlayerData Update"));
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("BTeam playerState is nullptr"));
		}
	}
}
