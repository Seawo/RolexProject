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

UClass* ARolexGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	UE_LOG(LogTemp, Warning, TEXT("Main Level ------------------------------------------------------------------------ Choose Player ------------------------------------------------------------------------"));
	
	ARolexPlayerState* RolexPlayerState = InController->GetPlayerState<ARolexPlayerState>();
	URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetGameInstance());

	if (RolexPlayerState && RolexGameInstance)
	{
		// print PlayerHeroSelection TMap
		for (auto Pair : RolexGameInstance->PlayerHeroSelections)
		{
			FString Mapkey = Pair.Key;
			if (*Mapkey)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *Mapkey);
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

		RolexPlayerState->FindUniqueID();
		if (TSubclassOf<ABaseCharacter>* BaseCharacterFactory =
			RolexGameInstance->PlayerHeroSelections.Find(RolexPlayerState->UniqueID))
		{
			return *BaseCharacterFactory;	
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to find BaseCharacter"));
		}
	}
	
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}
