// Copyright Epic Games, Inc. All Rights Reserved.

#include "RolexGameMode.h"

#include "BaseCharacter.h"
#include "RolexGameInstance.h"
#include "RolexProjectCharacter.h"
#include "RolexPlayerController.h"
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
	UE_LOG(LogTemp, Warning, TEXT("------------ RolexGameMode BeginPlay ------------"));
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
					// ARolexPlayerState* RolexPlayerState = Cast<ARolexPlayerState>(BaseCharacter->GetPlayerState());
					//BaseCharacter->Data.Team = RolexPlayerState->Team;
					UE_LOG(LogTemp, Warning, TEXT("[GameMode BeginPlay] Name : %s, Team : %s"), *BaseCharacter->GetName(), BaseCharacter->Data.Team ? TEXT("ATeam") : TEXT("BTeam"));
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
	UE_LOG(LogTemp, Warning, TEXT("------------ RolexGameMode GetDefaultPawnClassForController ------------"));
	
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
	UE_LOG(LogTemp, Warning, TEXT("[GameMode PostLogin]"));

	if (RolexGameInstance->PlayerTeam.Find(RolexPlayerState->UniqueID))
	{
		RolexPlayerState->Team = *RolexGameInstance->PlayerTeam.Find(RolexPlayerState->UniqueID);
		UE_LOG(LogTemp, Warning, TEXT("[GameMode PostLogin] Player : %s, Team : %s"), *NewPlayer->GetPawn()->GetName(), RolexPlayerState->Team ? TEXT("ATeam") : TEXT("BTeam"));

		// True == A Team, False == B Team
		TArray<AActor*> PlayerStartArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStartArray);

		// set player start for the player
		for (AActor* PlayerStart : PlayerStartArray)
		{
			if (RolexPlayerState and RolexPlayerState->Team) // A team starts in PlayerStart_4
			{
				if (PlayerStart->GetName().Contains(TEXT("PlayerStart_4")))
				{
					ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(NewPlayer->GetPawn());
					if (BaseCharacter)
					{
						BaseCharacter->Data.Team = true;
						BaseCharacter->SetActorLocation(PlayerStart->GetActorLocation());

						// team color
						UTeamWidget* TeamWidget = Cast<UTeamWidget>(BaseCharacter->TeamWidget->GetWidget());
						if (TeamWidget)
						{
							if ( TeamWidget->TeamColorImage)
								TeamWidget->TeamColorImage->SetColorAndOpacity(FLinearColor::Blue);
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Team Widget is NULL"));
						}
					}
				}
			}
			else if (RolexPlayerState and !RolexPlayerState->Team) // B team starts in PlayerStart_3
			{
				if (PlayerStart->GetName().Contains(TEXT("PlayerStart_3")))
				{
					ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(NewPlayer->GetPawn());
					if (BaseCharacter)
					{
						BaseCharacter->Data.Team = false;
						BaseCharacter->SetActorLocation(PlayerStart->GetActorLocation());

						// team color
						UTeamWidget* TeamWidget = Cast<UTeamWidget>(BaseCharacter->TeamWidget->GetWidget());
						if (TeamWidget)
						{
							if ( TeamWidget->TeamColorImage)
								TeamWidget->TeamColorImage->SetColorAndOpacity(FLinearColor::Red);
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Team Widget is NULL"));
						}
					}
				}
			}
		}
		
	}
}
