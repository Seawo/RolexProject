// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateSessionUI.h"
#include "RolexGameInstance.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

void UCreateSessionUI::NativeConstruct()
{
	Super::NativeConstruct();

	ConfirmBtn->OnClicked.AddDynamic(this, &UCreateSessionUI::Confrim);
}

void UCreateSessionUI::Confrim()
{
	if (SessionNameBox->GetText().IsEmpty()) return;
	URolexGameInstance* RolexGameInstace = Cast<URolexGameInstance>(GetWorld()->GetGameInstance());
	RolexGameInstace->CreateSession(*SessionNameBox->GetText().ToString());
}
