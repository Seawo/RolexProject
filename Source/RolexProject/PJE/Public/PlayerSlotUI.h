// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerSlotUI.generated.h"


class UImage;
class UTextBlock;

UCLASS()
class ROLEXPROJECT_API UPlayerSlotUI : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(meta = (BindWidget))
	UImage* TeamColor;

	UPROPERTY(meta = (BindWidget))
	UImage* PlayerHeroImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerID;

	// UFUNCTION(Server, Reliable)
	// void ServerRPC_SetPlayerHeroImage(UTexture2D* PlayerHeroTexture, int32 Index);
	//
	// UFUNCTION(NetMulticast, Reliable)
	// void MulticastRPC_SetPlayerHeroImage(UTexture2D* PlayerHeroTexture, int32 Index);

	UPROPERTY(ReplicatedUsing=OnRep_SetPlayerID)
	FString PlayerIDString;

	UFUNCTION()
	void OnRep_SetPlayerID();

	int32 PlayerSlotIndex;

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
