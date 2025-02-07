// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUI.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API ULobbyUI : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:
	// game start 
	UPROPERTY(meta = (BindWidget))
	class UButton* GameStartButton;

	UPROPERTY(meta = (BindWidget))
	UButton* OptionButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* LobbyWidgetSwitcher;

	UFUNCTION()
	void SwitchWidget();
	
	class URolexGameInstance* RolexGameInstace;

	// create session
	UPROPERTY(meta = (BindWidget))
	class UButton* CreateSessionBtn;

	UFUNCTION()
	void CreateSession();

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere)
	class UCreateSessionUI* CreateSessionUI;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere)
	class UMapSelectUI* MapSelectUI;
	
	UPROPERTY(meta = (BindWidget))
	UButton* FindSessionBtn;

	UFUNCTION()
	void FindSession();

	UPROPERTY(meta = (BindWidget))
	UButton* UndoButton;

	UFUNCTION()
	void UndoSwitchWidget();
	
	// found sessions scroll box 
	UPROPERTY(meta = (BindWidget))
	class UBorder* SessionScrollBoxBorder;
	
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* SessionScrollBox;

	UFUNCTION()
	void AddSession(int32 SessionIndex, const FString& Owner, const FString& Name);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class USessionItemUI> SessionItemUIFactory;

	UPROPERTY(meta = (BindWidget))
	class UOptionUI* OptionUI;

	UFUNCTION()
	void AddOptionUIToViewport();
public:
	UFUNCTION()
	void DisableButtonClick();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* LobbySound;

	UFUNCTION()
	void CloseOptionUI();
};
