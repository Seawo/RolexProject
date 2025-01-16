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
		//find selected hero class based on the UniqueID key
		RolexPlayerState->FindUniqueID();
		if (TSubclassOf<ABaseCharacter>* BaseCharacterFactory = RolexGameInstance->PlayerHeroSelections.Find(RolexPlayerState->UniqueID))
		{
			return *BaseCharacterFactory;	
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Failed to find BaseCharacter"));
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

// // print PlayerHeroSelection TMap
// for (auto Pair : RolexGameInstance->PlayerHeroSelections)
// {
// 	if (FString Mapkey = Pair.Key; *Mapkey)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Key: %s"), *Mapkey);
// 		Message = FString::Printf(TEXT("Key: %s"), *Mapkey);
// 		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, Message);
// 	}
// 	if (TSubclassOf<ABaseCharacter> MapValue = Pair.Value)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Name: %s"), *MapValue->GetName());
// 		Message = FString::Printf(TEXT("Name: %s"), *MapValue->GetName());
// 		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, Message);
// 	}
// }