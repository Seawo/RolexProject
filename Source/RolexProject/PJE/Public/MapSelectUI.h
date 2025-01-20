// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapSelectUI.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API UMapSelectUI : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* LeftButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* RightButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, EditAnywhere)
	UButton* MedivalAsia;
	
	UFUNCTION()
	void OnClickedMedivalAsia();
};
