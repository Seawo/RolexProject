// Fill out your copyright notice in the Description page of Project Settings.


#include "PJE/Public/SessionItemUI.h"

#include "RolexGameInstance.h"
#include "Components/Button.h"

void USessionItemUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (SessionJoinBtn)
		SessionJoinBtn->OnClicked.AddDynamic(this, &USessionItemUI::JoinSession);
}

void USessionItemUI::JoinSession()
{
	URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetWorld()->GetGameInstance());

	if (RolexGameInstance)
		RolexGameInstance->JoinSession(SessionIndex);
}
