// Fill out your copyright notice in the Description page of Project Settings.


#include "PJE/Public/LobbyUI.h"

#include "NavigationSystemTypes.h"
#include "RolexGameInstance.h"
#include "SessionItemUI.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void ULobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	RolexGameInstace = Cast<URolexGameInstance>(GetWorld()->GetGameInstance());

	// bind function to OnClicked delegate
	CreateSessionBtn->OnClicked.AddDynamic(this, &ULobbyUI::CreateSession);
	FindSessionBtn->OnClicked.AddDynamic(this, &ULobbyUI::FindSession);

	if (RolexGameInstace)
		RolexGameInstace->AddSession.BindUObject(this, &ULobbyUI::AddSession);
}

void ULobbyUI::CreateSession()
{
	UCreateSessionUI* CreateSessionUI = Cast<UCreateSessionUI>(CreateWidget(this, CreateSessionUIFactory));
	if (CreateSessionUI)
	{
		CreateSessionUI->AddToViewport();
	}
}

void ULobbyUI::FindSession()
{
	RolexGameInstace->FindSession();
}

void ULobbyUI::AddSession(int32 SessionIndex, const FString& Owner, const FString& Name)
{
	USessionItemUI* SessionItem = Cast<USessionItemUI>(CreateWidget(this, SessionItemUIFactory));

	SessionItem->SessionIndex = SessionIndex;
	SessionItem->Owner->SetText(FText::FromString(Owner));
	SessionItem->RoomName->SetText(FText::FromString(Name));
	
	SessionScrollBox->AddChild(SessionItem);
}
