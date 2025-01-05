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

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* SessionNameBox;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmBtn;
};
