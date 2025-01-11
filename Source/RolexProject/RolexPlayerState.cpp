// Fill out your copyright notice in the Description page of Project Settings.


#include "RolexPlayerState.h"

void ARolexPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	ARolexPlayerState* RolexPlayerState = Cast<ARolexPlayerState>(PlayerState);
	if (RolexPlayerState)
	{
		RolexPlayerState->SelectedHero = SelectedHero;
	}
}
