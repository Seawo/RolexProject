// Copyright Epic Games, Inc. All Rights Reserved.

#include "RolexGameMode.h"

#include "BaseCharacter.h"
#include "RolexGameInstance.h"
#include "RolexProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARolexGameMode::ARolexGameMode()
{
	bUseSeamlessTravel = true;
	
	// // set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	//
	// if (PlayerPawnBPClass.Class != NULL)
	// {
	// 	DefaultPawnClass = PlayerPawnBPClass.Class;
	// }
}

AActor* ARolexGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("Main Level --------- Choose Player ---------"));
	
	ARolexPlayerState* RolexPlayerState = Player->GetPlayerState<ARolexPlayerState>();
	URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetGameInstance());

	if (RolexPlayerState && RolexGameInstance)
	{
		for (auto Pair : RolexGameInstance->PlayerHeroSelections)
		{
			ARolexPlayerState* Mapkey = Pair.Key;
			if (Mapkey)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *Mapkey->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to find player state"));
			}
			TSubclassOf<ABaseCharacter> MapValue = Pair.Value;
			if (MapValue)
				UE_LOG(LogTemp, Warning, TEXT("%s"), *MapValue->GetName());
		}
		UE_LOG(LogTemp, Warning, TEXT("Player State Name: %s"), *RolexPlayerState->GetName());
		
		if (TSubclassOf<ABaseCharacter>* BaseCharacterFactory =
			RolexGameInstance->PlayerHeroSelections.Find(RolexPlayerState))
		{
			FString CharacterName = BaseCharacterFactory->Get()->GetName();
			UE_LOG(LogTemp, Warning, TEXT("Character: %s"), *CharacterName);
			Player->Possess(GetWorld()->SpawnActor<ABaseCharacter>(*BaseCharacterFactory,
				FVector::ZeroVector, FRotator::ZeroRotator));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to find BaseCharacter"));
		}
	}
	
	return Super::ChoosePlayerStart_Implementation(Player); 
}
