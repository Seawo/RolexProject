// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyUI.h"

#include "AudioManager.h"
#include "Blueprint/UserWidget.h"
#include "RolexGameInstance.h"
#include "SessionItemUI.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "CreateSessionUI.h"
#include "MapSelectUI.h"
#include "Components/AudioComponent.h"
#include "Components/Border.h"
#include "Components/WidgetSwitcher.h"
#include "OptionUI.h"

class AAudioManager;

void ULobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	CreateSessionUI->SetVisibility(ESlateVisibility::Hidden);
	MapSelectUI->SetVisibility(ESlateVisibility::Hidden);
	SessionScrollBoxBorder->SetVisibility(ESlateVisibility::Hidden);
	OptionUI->SetVisibility(ESlateVisibility::Hidden);
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController ->bShowMouseCursor = true;
	}

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ConsoleCommand(TEXT("r.ScreenPercentage 0"));
	
	RolexGameInstace = Cast<URolexGameInstance>(GetWorld()->GetGameInstance());

	// bind function to OnClicked delegate
	GameStartButton->OnClicked.AddDynamic(this, &ULobbyUI::SwitchWidget);
	UndoButton->OnClicked.AddDynamic(this, &ULobbyUI::UndoSwitchWidget);
	CreateSessionBtn->OnClicked.AddDynamic(this, &ULobbyUI::CreateSession);
	FindSessionBtn->OnClicked.AddDynamic(this, &ULobbyUI::FindSession);
	OptionButton->OnClicked.AddDynamic(this, &ULobbyUI::AddOptionUIToViewport);
	OptionUI->CloseButton->OnClicked.AddDynamic(this, &ULobbyUI::CloseOptionUI);

	if (RolexGameInstace)
		RolexGameInstace->AddSession.BindUObject(this, &ULobbyUI::AddSession);

	// set sound
	AAudioManager* AudioManager = Cast<AAudioManager>(UGameplayStatics::GetActorOfClass(this, AAudioManager::StaticClass()));
	if (AudioManager && LobbySound)
	{
		//bAutoActivate
		AudioManager->AudioComponent->SetSound(LobbySound);
		AudioManager->AudioComponent->Play();
	}
}

void ULobbyUI::SwitchWidget()
{
	if (LobbyWidgetSwitcher)
		LobbyWidgetSwitcher->SetActiveWidgetIndex(1);
}

void ULobbyUI::CreateSession()
{
	MapSelectUI->SetVisibility(ESlateVisibility::Visible);
	CreateSessionUI->SetVisibility(ESlateVisibility::Visible);
}

void ULobbyUI::FindSession()
{
	SessionScrollBoxBorder->SetVisibility(ESlateVisibility::Visible);

	// clear SessionScrollBox
	SessionScrollBox->ClearChildren();
	
	RolexGameInstace->FindSession();
}

void ULobbyUI::UndoSwitchWidget()
{
	if (MapSelectUI->IsVisible())
		MapSelectUI->SetVisibility(ESlateVisibility::Hidden);

	if (CreateSessionUI->IsVisible())
		CreateSessionUI->SetVisibility(ESlateVisibility::Hidden);
	
	// find session scroll box
	if (SessionScrollBoxBorder->IsVisible())
		SessionScrollBoxBorder->SetVisibility(ESlateVisibility::Hidden);
	
	if (LobbyWidgetSwitcher)
		LobbyWidgetSwitcher->SetActiveWidgetIndex(0);
}

void ULobbyUI::AddSession(int32 SessionIndex, const FString& Owner, const FString& Name)
{
	USessionItemUI* SessionItem = Cast<USessionItemUI>(CreateWidget(this, SessionItemUIFactory));

	SessionItem->SessionIndex = SessionIndex;
	SessionItem->Owner->SetText(FText::FromString(Owner));
	SessionItem->RoomName->SetText(FText::FromString(Name));
	SessionItem->SessionJoinBtn->OnClicked.AddDynamic(this, &ULobbyUI::DisableButtonClick);
	
	SessionScrollBox->AddChild(SessionItem);
}

// make other buttons disabled when the player click join button
void ULobbyUI::DisableButtonClick()
{
	GameStartButton->SetIsEnabled(false);
	OptionButton->SetIsEnabled(false);
	QuitButton->SetIsEnabled(false);
	CreateSessionBtn->SetIsEnabled(false);
	FindSessionBtn->SetIsEnabled(false);
	UndoButton->SetIsEnabled(false);
}

void ULobbyUI::AddOptionUIToViewport()
{
	OptionUI->SetVisibility(ESlateVisibility::Visible);
	GameStartButton->SetIsEnabled(false);
}

void ULobbyUI::CloseOptionUI()
{
	OptionUI->SetVisibility(ESlateVisibility::Hidden);
	GameStartButton->SetIsEnabled(true);
}
