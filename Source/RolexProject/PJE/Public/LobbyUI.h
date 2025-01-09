// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CreateSessionUI.h"
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
	class URolexGameInstance* RolexGameInstace;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* CreateSessionBtn;

	UFUNCTION()
	void CreateSession();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCreateSessionUI> CreateSessionUIFactory;
	
	UPROPERTY(meta = (BindWidget))
	UButton* FindSessionBtn;

	UFUNCTION()
	void FindSession();

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* SessionScrollBox;

	UFUNCTION()
	void AddSession(int32 SessionIndex, const FString& Owner, const FString& Name);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class USessionItemUI> SessionItemUIFactory;
	
};
