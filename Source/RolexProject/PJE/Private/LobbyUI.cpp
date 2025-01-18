// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyUI.h"
#include "Blueprint/UserWidget.h"
#include "NavigationSystemTypes.h"
#include "RolexGameInstance.h"
#include "SessionItemUI.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "CreateSessionUI.h"
#include "MapSelectUI.h"
#include "Components/Border.h"

void ULobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	CreateSessionUI->SetVisibility(ESlateVisibility::Hidden);
	MapSelectUI->SetVisibility(ESlateVisibility::Hidden);
	SessionScrollBoxBorder->SetVisibility(ESlateVisibility::Hidden);
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController ->bShowMouseCursor = true;
	}

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ConsoleCommand(TEXT("r.ScreenPercentage 0"));
	
	RolexGameInstace = Cast<URolexGameInstance>(GetWorld()->GetGameInstance());

	// bind function to OnClicked delegate
	CreateSessionBtn->OnClicked.AddDynamic(this, &ULobbyUI::CreateSession);
	FindSessionBtn->OnClicked.AddDynamic(this, &ULobbyUI::FindSession);

	if (RolexGameInstace)
		RolexGameInstace->AddSession.BindUObject(this, &ULobbyUI::AddSession);
}

void ULobbyUI::CreateSession()
{
	MapSelectUI->SetVisibility(ESlateVisibility::Visible);
	CreateSessionUI->SetVisibility(ESlateVisibility::Visible);
}

void ULobbyUI::FindSession()
{
	SessionScrollBoxBorder->SetVisibility(ESlateVisibility::Visible);
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
