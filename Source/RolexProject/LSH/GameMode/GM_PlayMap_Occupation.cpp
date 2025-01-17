// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_PlayMap_Occupation.h"

#include "BaseCharacter.h"
#include "RolexPlayerController.h"
#include "RolexGameInstance.h"


AGM_PlayMap_Occupation::AGM_PlayMap_Occupation()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseSeamlessTravel = true;
}

void AGM_PlayMap_Occupation::BeginPlay()
{
	Super::BeginPlay();


}

void AGM_PlayMap_Occupation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

UClass* AGM_PlayMap_Occupation::GetDefaultPawnClassForController_Implementation(AController* InController)
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
