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
	
	// // set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	//
	// if (PlayerPawnBPClass.Class != NULL)
	// {
	// 	DefaultPawnClass = PlayerPawnBPClass.Class;
	// }
}

void ARolexGameMode::BeginPlay()
{
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
					UE_LOG(LogTemp, Log, TEXT("[GameMode BeginPlay] player Team : %s"), rolexPS->Team ? TEXT("ATeam") : TEXT("BTeam"));

					UE_LOG(LogTemp, Warning, TEXT("[GameMode BeginPlay] player Name : %s"), *p->GetName());

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

AActor* ARolexGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	// True == A팀, False == B팀
	TArray<AActor*> foundPlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), foundPlayerStarts);
	for (AActor* playerStart : foundPlayerStarts)
	{
		ABaseCharacter* player = Cast<ABaseCharacter>(Player->GetPawn());
		ARolexPlayerState* RolexPlayerState = Player->GetPlayerState<ARolexPlayerState>();
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