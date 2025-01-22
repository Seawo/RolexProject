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

void ARolexPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	ARolexPlayerState* RolexPlayerState = Cast<ARolexPlayerState>(PlayerState);
	if (RolexPlayerState)
	{
		RolexPlayerState->SelectedHero = SelectedHero;
		RolexPlayerState->Team = Team;
	}
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
