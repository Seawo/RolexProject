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
	
	FTimerHandle timerPlayer;
	GetWorldTimerManager().SetTimer(timerPlayer, [this]()
	{
			TArray<AActor*> foundPlayer;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), foundPlayer);
			for (AActor* player : foundPlayer)
			{
				ABaseCharacter* p = Cast<ABaseCharacter>(player);
				if (p)
				{
					ARolexPlayerState* rolexPS = Cast<ARolexPlayerState>(p->GetPlayerState());
					UE_LOG(LogTemp, Warning, TEXT("[GameMode BeginPlay] player Name : %s"), *p->GetName());
					UE_LOG(LogTemp, Warning, TEXT("[GameMode BeginPlay] player Team : %s"), rolexPS->Team ? TEXT("ATeam") : TEXT("BTeam"));
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

AActor* ARolexGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("------------------------------------------------------------------------ RolexGameMode ChoosePlayerStart ------------------------------------------------------------------------"));
	// set team
	URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetGameInstance());
	if (RolexGameInstance)
		UE_LOG(LogTemp, Warning, TEXT("[GameMode ChoosePlayerStart] RolexGameInstance exists"));
	
	ARolexPlayerState* RolexPlayerState = Player->GetPlayerState<ARolexPlayerState>();
	if (RolexGameInstance->PlayerTeam.Find(RolexPlayerState->UniqueID))
	{
		UE_LOG(LogTemp, Warning, TEXT("[GameMode ChoosePlayerStart] Player ID : %i"), RolexPlayerState->GetPlayerId());
		UE_LOG(LogTemp, Warning, TEXT("Assign team "));
		RolexPlayerState->Team = *RolexGameInstance->PlayerTeam.Find(RolexPlayerState->UniqueID);
	}
	
	// True == A팀, False == B팀
	TArray<AActor*> foundPlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), foundPlayerStarts);
	for (AActor* playerStart : foundPlayerStarts)
	{
		UE_LOG(LogTemp, Warning, TEXT("[GameMode ChoosePlayerStart] playerStart Name : %s"), *playerStart->GetName());
		UE_LOG(LogTemp, Warning, TEXT("[GameMode ChoosePlayerStart] Team : %s"), RolexPlayerState->Team ? TEXT("ATeam") : TEXT("BTeam"));
		if (RolexPlayerState and RolexPlayerState->Team) // A팀
		{
			if (playerStart->GetName().Contains(TEXT("PlayerStart_4")))
			{
				return playerStart;
			}
		}
		else if (RolexPlayerState and not RolexPlayerState->Team) // B팀
		{
			if (playerStart->GetName().Contains(TEXT("PlayerStart_3")))
			{
				return playerStart;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("player is nullptr"));
		}
	}

	return nullptr;
}

void ARolexGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Warning, TEXT("------------------------------------------------------------------------ RolexGameMode PostLogin ------------------------------------------------------------------------"));
	ChoosePlayerStart_Implementation(NewPlayer);
}
