// Fill out your copyright notice in the Description page of Project Settings.


#include "RolexPlayerState.h"

#include "Net/UnrealNetwork.h"

ARolexPlayerState::ARolexPlayerState()
{
	// get ID
	FUniqueNetIdRepl NetId = GetUniqueId();
	if (NetId.IsValid())
	{
		UniqueID = NetId->ToString();
		UE_LOG(LogTemp, Warning, TEXT("UniqueID: %s"), *UniqueID);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get unique ID"));
	}
}

void ARolexPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
}

// void ARolexPlayerState::CopyProperties(APlayerState* PlayerState)
// {
// 	Super::CopyProperties(PlayerState);
//
// 	ARolexPlayerState* RolexPlayerState = Cast<ARolexPlayerState>(PlayerState);
// 	if (RolexPlayerState)
// 	{
// 		RolexPlayerState->SelectedHero = SelectedHero;
// 		RolexPlayerState->Team = Team;
// 	}
// }

void ARolexPlayerState::ServerSetUniqueID_Implementation(const FString& ID)
{
	UniqueID = ID;
}

void ARolexPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARolexPlayerState, Team);
}

void ARolexPlayerState::FindUniqueID()
{
	// get ID
	FUniqueNetIdRepl NetId = GetUniqueId();
	if (NetId.IsValid())
	{
		UniqueID = NetId->ToString();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get unique ID"));
	}
	
}
void ARolexPlayerState::ServerPlayerKillCount_Implementation()
{
	MultiPlayerKillCount(1);
}

void ARolexPlayerState::ServerPlayerDeathCount_Implementation()
{
	MultiPlayerDeathCount(1);
}

void ARolexPlayerState::ServerPlayerDamage_Implementation(int damage)
{
	MultiPlayerDamage(damage);
}

void ARolexPlayerState::ServerPlayerHealing_Implementation(int healing)
{
	MultiPlayerHealing(healing);
}

void ARolexPlayerState::MultiPlayerKillCount_Implementation(int count)
{
	PlayerData.KillCount += count;
}

void ARolexPlayerState::MultiPlayerDeathCount_Implementation(int count)
{
	PlayerData.DeathCount += count;
}

void ARolexPlayerState::MultiPlayerDamage_Implementation(int damage)
{
	PlayerData.Damage += damage;
}

void ARolexPlayerState::MultiPlayerHealing_Implementation(int healing)
{
	PlayerData.Healing += healing;
}
