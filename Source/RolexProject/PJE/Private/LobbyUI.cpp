// Fill out your copyright notice in the Description page of Project Settings.


#include "PJE/Public/LobbyUI.h"

#include "NavigationSystemTypes.h"
#include "RolexGameInstance.h"
#include "Components/Button.h"

void ULobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	RolexGameInstace = Cast<URolexGameInstance>(GetWorld()->GetGameInstance());

	// bind function to OnClicked delegate
	CreateSessionBtn->OnClicked.AddDynamic(this, &ULobbyUI::CreateSession);
	FindSessionBtn->OnClicked.AddDynamic(this, &ULobbyUI::FindSession);
}

void ULobbyUI::CreateSession()
{
	RolexGameInstace->CreateSession();
}

void ULobbyUI::FindSession()
{
}

void ULobbyUI::AddSession()
{
}
