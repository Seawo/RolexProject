// Copyright Epic Games, Inc. All Rights Reserved.

#include "RolexGameMode.h"

#include "BaseCharacter.h"
#include "RolexGameInstance.h"
#include "RolexProjectCharacter.h"
#include "RolexPlayerController.h"
#include "RolexPlayerState.h"
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
	UE_LOG(LogTemp, Warning, TEXT("------------------------------------------------------------------------ RolexGameMode BeginPlay ------------------------------------------------------------------------"));
	Super::BeginPlay();

	// print player name and team
	FTimerHandle timerPlayer;
	GetWorldTimerManager().SetTimer(timerPlayer, [this]()
	{
			TArray<AActor*> PlayerArray;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), PlayerArray);
			for (AActor* Player : PlayerArray)
			{
				ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Player);
				if (BaseCharacter)
				{
					ARolexPlayerState* RolexPlayerState = Cast<ARolexPlayerState>(BaseCharacter->GetPlayerState());
					BaseCharacter->Data.Team = RolexPlayerState->Team;
					UE_LOG(LogTemp, Warning, TEXT("[GameMode BeginPlay] Name : %s, Team : %s"), *BaseCharacter->GetName(), RolexPlayerState->Team ? TEXT("ATeam") : TEXT("BTeam"));
				}
			}
	}, 3.0f, false);
}

void ARolexGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UClass* ARolexGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	UE_LOG(LogTemp, Warning, TEXT("------------------------------------------------------------------------ RolexGameMode GetDefaultPawnClassForController ------------------------------------------------------------------------"));
	
	URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetGameInstance());
	ARolexPlayerState* RolexPlayerState = InController->GetPlayerState<ARolexPlayerState>();

	if (RolexPlayerState && RolexGameInstance)
	{
		//find selected hero class based on the UniqueID key
		RolexPlayerState->FindUniqueID();
		if (TSubclassOf<ABaseCharacter>* BaseCharacterFactory = RolexGameInstance->PlayerHeroSelections.Find(RolexPlayerState->UniqueID))
		{
			UE_LOG(LogTemp, Warning, TEXT("[GameMode GetDefaultPawnClassForController] Player ID : %s"), *RolexPlayerState->UniqueID);
			return *BaseCharacterFactory;	
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Failed to find BaseCharacter"));
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ARolexGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetGameInstance());
	ARolexPlayerState* RolexPlayerState = NewPlayer->GetPlayerState<ARolexPlayerState>();
	
	if (RolexGameInstance->PlayerTeam.Find(RolexPlayerState->UniqueID))
	{
		RolexPlayerState->Team = *RolexGameInstance->PlayerTeam.Find(RolexPlayerState->UniqueID);
		
		// True == A팀, False == B팀
		TArray<AActor*> PlayerStartArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStartArray);

		// set player start for the player
		for (AActor* PlayerStart : PlayerStartArray)
		{
			if (RolexPlayerState and RolexPlayerState->Team) // A team starts in PlayerStart_4
			{
				if (PlayerStart->GetName().Contains(TEXT("PlayerStart_4")))
				{
					APawn* Pawn = NewPlayer->GetPawn();
					if (Pawn)
						Pawn->SetActorLocation(PlayerStart->GetActorLocation());
				}
			}
			else if (RolexPlayerState and !RolexPlayerState->Team) // B team starts in PlayerStart_3
			{
				if (PlayerStart->GetName().Contains(TEXT("PlayerStart_3")))
				{
					APawn* Pawn = NewPlayer->GetPawn();
					if (Pawn)
						Pawn->SetActorLocation(PlayerStart->GetActorLocation());
				}
			}
		}
	}
}
