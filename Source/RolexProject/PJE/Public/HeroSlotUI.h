// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HeroSlotUI.generated.h"


DECLARE_DELEGATE_TwoParams(FHeroSelectButtonClikcedDelegate, UTexture2D*, int32);

UCLASS()
class ROLEXPROJECT_API UHeroSlotUI : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* HeroTexture;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	class UImage* HeroImage;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* HeroSelectButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HeroInfo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText HeroDescription;
	
	FHeroSelectButtonClikcedDelegate HeroSelectButtonClickedDelegate;

	UFUNCTION()
	void OnHeroSelectButtonClicked();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABaseCharacter> BaseCharacter;

	int32 OwnerPlayerSlotIndex;

	class AWaitingRoomGameStateBase*  WaitingRoomGameStateBase;

	class ARolexPlayerController* RolexPlayerController;
};
