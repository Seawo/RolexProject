// Copyright Epic Games, Inc. All Rights Reserved.

#include "RolexGameMode.h"

#include "BaseCharacter.h"
#include "RolexGameInstance.h"
#include "RolexProjectCharacter.h"
#include "RolexPlayerController.h"
#include "GameState/GS_TrainingRoom.h"
#include "RolexPlayerState.h"
#include "TeamWidget.h"
#include "Components/Image.h"
#include "Components/WidgetComponent.h"
#include "GameState/GS_TrainingRoom.h"
#include "Kismet/GameplayStatics.h"
#include "Point/Actor_FightPoint.h"
#include "GameFramework/PlayerStart.h"

#include "UObject/ConstructorHelpers.h"

ARolexGameMode::ARolexGameMode()
{
	// set default pawn class to our Blueprinted character
	PrimaryActorTick.bCanEverTick = true;
	bUseSeamlessTravel = true;
	
}

void ARolexGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("------------ RolexGameMode BeginPlay ------------"));


	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, [this]()
		{
			URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetGameInstance());

			TArray<AActor*> PlayerStartArray;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStartArray);

			TArray<AActor*> foundCharacters;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), foundCharacters);

			UE_LOG(LogTemp, Error, TEXT("[RolexGameMode BeginPlay] foundCharacters.Num(): %d"), foundCharacters.Num());

			for (AActor* character : foundCharacters)
			{
				ABaseCharacter* baseCharacter = Cast<ABaseCharacter>(character);
				ARolexPlayerState* RolexPlayerState = Cast<ARolexPlayerState>(baseCharacter->GetController()->PlayerState);
				if (RolexPlayerState)
				{
					if (RolexGameInstance->PlayerTeam.Contains(RolexPlayerState->UniqueID))
					{
						baseCharacter->Data.Team = *RolexGameInstance->PlayerTeam.Find(RolexPlayerState->UniqueID);
						UE_LOG(LogTemp, Error, TEXT("[RolexGameMode BeginPlay] BaseCharacter->Data.Team: %d"), baseCharacter->Data.Team);

						for (AActor* playerStart : PlayerStartArray)
						{
							if (baseCharacter->Data.Team)
							{
								if (playerStart->GetName().Contains(TEXT("PlayerStart_4")))
								{
									baseCharacter->SetActorLocation(playerStart->GetActorLocation());
								}
							}
							else
							{
								if (playerStart->GetName().Contains(TEXT("PlayerStart_3")))
								{
									baseCharacter->SetActorLocation(playerStart->GetActorLocation());
								}
							}
						}
					}
				}
				

			}
		}, 5.0f, false);
}

void ARolexGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UClass* ARolexGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	UE_LOG(LogTemp, Warning, TEXT("------------ RolexGameMode GetDefaultPawnClassForController ------------"));
	
	URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetGameInstance());
	ARolexPlayerState* RolexPlayerState = InController->GetPlayerState<ARolexPlayerState>();

	if (RolexPlayerState && RolexGameInstance)
	{
		//find selected hero class based on the UniqueID key
		RolexPlayerState->FindUniqueID();
		if (TSubclassOf<ABaseCharacter>* BaseCharacterFactory = RolexGameInstance->PlayerHeroSelections.Find(RolexPlayerState->UniqueID))
		{
			/*BaseCharacterFactory->GetDefaultObject()->Data.Team = RolexGameInstance->PlayerTeam[RolexPlayerState->UniqueID];*/

			UE_LOG(LogTemp, Warning, TEXT("[GameMode GetDefaultPawnClassForController] Player ID : %s"), *RolexPlayerState->UniqueID);
			return *BaseCharacterFactory;	
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Failed to find BaseCharacter"));
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* ARolexGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	UE_LOG(LogTemp, Warning, TEXT("------------ RolexGameMode SpawnDefaultPawnFor ------------"));

	return GetWorld()->SpawnActor<ABaseCharacter>(GetDefaultPawnClassForController(NewPlayer),
													StartSpot->GetActorLocation(), 
													StartSpot->GetActorRotation());
}

//AActor* ARolexGameMode::ChoosePlayerStart_Implementation(AController* Player)
//{
//	TArray<AActor*> PlayerStartArray;
//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStartArray);
//
//	for (AActor* playerStart : PlayerStartArray)
//	{
//		ABaseCharacter* character = Cast<ABaseCharacter>(Player->GetPawn());
//		if (character->Data.Team)
//		{
//			if (playerStart->GetName() == TEXT("PlayerStart_4"))
//			{
//				return playerStart;
//			}
//		}
//		else
//		{
//			if (playerStart->GetName().Contains(TEXT("PlayerStart_3")))
//			{
//				return playerStart;
//			}
//		}
//	}
//
//
//
//
//	return Super::ChoosePlayerStart_Implementation(Player);
//}



void ARolexGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetGameInstance());
	ARolexPlayerState* RolexPlayerState = NewPlayer->GetPlayerState<ARolexPlayerState>();
	UE_LOG(LogTemp, Warning, TEXT("[RolexGameMode PostLogin]"));
	
	if (RolexGameInstance->PlayerTeam.Find(RolexPlayerState->UniqueID))
	{
		RolexPlayerState->Team = *RolexGameInstance->PlayerTeam.Find(RolexPlayerState->UniqueID);
		UE_LOG(LogTemp, Warning, TEXT("[RolexGameMode PostLogin] GI Player : %s, Team : %s"), *NewPlayer->GetPawn()->GetName(), RolexGameInstance->PlayerTeam[RolexPlayerState->UniqueID] ? TEXT("ATeam") : TEXT("BTeam"));
		UE_LOG(LogTemp, Warning, TEXT("[RolexGameMode PostLogin] PS Player : %s, Team : %s"), *NewPlayer->GetPawn()->GetName(), RolexPlayerState->Team ? TEXT("ATeam") : TEXT("BTeam"));

		// True == A Team, False == B Team
		TArray<AActor*> PlayerStartArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStartArray);

		// set player start for the player
		for (AActor* PlayerStart : PlayerStartArray)
		{
			if (RolexPlayerState and RolexGameInstance->PlayerTeam[RolexPlayerState->UniqueID]) // A team starts in PlayerStart_4
			{
				if (PlayerStart->GetName().Contains(TEXT("PlayerStart_4")))
				{
					ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(NewPlayer->GetPawn());
					if (BaseCharacter)
					{
						BaseCharacter->Data.Team = RolexGameInstance->PlayerTeam[RolexPlayerState->UniqueID];
						UE_LOG(LogTemp, Warning, TEXT("[RolexGameMode PostLogin] BaseCharacter->Data.Team: %d"), BaseCharacter->Data.Team);
						BaseCharacter->SetActorLocation(PlayerStart->GetActorLocation());
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("[RolexGameMode PostLogin] No BaseCharacter"));
					}
				}
			}
			else if (RolexPlayerState and !RolexGameInstance->PlayerTeam[RolexPlayerState->UniqueID]) // B team starts in PlayerStart_3
			{
				if (PlayerStart->GetName().Contains(TEXT("PlayerStart_3")))
				{
					ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(NewPlayer->GetPawn());
					if (BaseCharacter)
					{
						BaseCharacter->Data.Team = RolexGameInstance->PlayerTeam[RolexPlayerState->UniqueID];
						UE_LOG(LogTemp, Warning, TEXT("[RolexGameMode PostLogin] BaseCharacter->Data.Team: %d"), BaseCharacter->Data.Team);
						BaseCharacter->SetActorLocation(PlayerStart->GetActorLocation());
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("[RolexGameMode PostLogin] No BaseCharacter"));
					}
				}
			}
		}
		
	}
}
