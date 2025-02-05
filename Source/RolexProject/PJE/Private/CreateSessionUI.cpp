// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateSessionUI.h"
#include "RolexGameInstance.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ComboBoxString.h"

void UCreateSessionUI::NativeConstruct()
{
	Super::NativeConstruct();

	ConfirmBtn->OnClicked.AddDynamic(this, &UCreateSessionUI::Confirm);
}

void UCreateSessionUI::Confirm()
{
	if (SessionNameBox->GetText().IsEmpty()) return;

	// session settings
	int32 Num = FCString::Atoi(*NumberOfPlayers->GetSelectedOption());
	if (!Num)
		return;
	
	URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetGameInstance());
	if (RolexGameInstance)
	{
		RolexGameInstance->MaxPlayersNum = Num;
	}

	UE_LOG(LogTemp, Warning, TEXT("[UCreateSessionUI] Session Name: %s"), *SessionNameBox->GetText().ToString());
	RolexGameInstance->RoomName = SessionNameBox->GetText();
	UE_LOG(LogTemp, Warning, TEXT("[UCreateSessionUI] RolexGameInstance->RoomName: %s"), *SessionNameBox->GetText().ToString());
	// create a new sessions
	RolexGameInstance->CreateSession(*SessionNameBox->GetText().ToString());
}
