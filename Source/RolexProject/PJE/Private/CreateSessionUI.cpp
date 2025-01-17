// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateSessionUI.h"
#include "RolexGameInstance.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ComboBoxString.h"

void UCreateSessionUI::NativeConstruct()
{
	Super::NativeConstruct();

	ConfirmBtn->OnClicked.AddDynamic(this, &UCreateSessionUI::Confrim);
}

void UCreateSessionUI::Confrim()
{
	if (SessionNameBox->GetText().IsEmpty()) return;

	// session settings
	int32 Num = FCString::Atoi(*NumberofPlayers->GetSelectedOption());
	if (!Num)
		return;
	
	URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetGameInstance());
	if (RolexGameInstance)
	{
		RolexGameInstance->MaxPlayersNum = Num;
	}
	
	// create a new session
	URolexGameInstance* RolexGameInstace = Cast<URolexGameInstance>(GetWorld()->GetGameInstance());
	RolexGameInstace->CreateSession(*SessionNameBox->GetText().ToString());
}
