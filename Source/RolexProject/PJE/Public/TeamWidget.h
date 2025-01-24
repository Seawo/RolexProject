// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TeamWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API UTeamWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* TeamColorImage;
};
