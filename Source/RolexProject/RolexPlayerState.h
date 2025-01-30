// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RolexPlayerState.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()

	// 이미지

	UPROPERTY(VisibleAnywhere, Category = "Info")
	int32 KillCount;

	UPROPERTY(VisibleAnywhere, Category = "Info")
	int32 DeathCount;

	UPROPERTY(VisibleAnywhere, Category = "Info")
	int32 Damage;

	UPROPERTY(VisibleAnywhere, Category = "Info")
	int32 Healing;
};

UCLASS()
class ROLEXPROJECT_API ARolexPlayerState : public APlayerState
{
	GENERATED_BODY()

	ARolexPlayerState();
	
public:
	virtual void BeginPlay() override;
	
	TSubclassOf<class ABaseCharacter> SelectedHero;

	//virtual void CopyProperties(APlayerState* PlayerState) override;
	
	FString UniqueID;

	UFUNCTION(Server, Reliable)
	void ServerSetUniqueID(const FString& ID);
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
	bool Team;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void FindUniqueID();

	UPROPERTY(VisibleAnywhere, Category = "Info")
	FPlayerData PlayerData;
};

