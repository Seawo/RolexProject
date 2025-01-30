// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateSessionUI.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API UCreateSessionUI : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* SessionNameBox;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmBtn;

	UFUNCTION()
	void Confirm();

	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* NumberOfPlayers;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere)
	class UTextBlock* MapString;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MapName;
};
