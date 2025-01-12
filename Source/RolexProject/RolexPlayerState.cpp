// Fill out your copyright notice in the Description page of Project Settings.


#include "RolexPlayerState.h"

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
	}
}

void ARolexPlayerState::FindUniqueID()
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
