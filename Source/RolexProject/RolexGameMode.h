// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RolexGameMode.generated.h"


UCLASS(minimalapi)
class ARolexGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARolexGameMode();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, class AActor* StartSpot) override;

	//virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;




	UPROPERTY(VisibleAnywhere, Category = "Info")
	TArray<class ABaseCharacter*> ATeamChracters;		// Team == true (A팀)
	UPROPERTY(VisibleAnywhere, Category = "Info")
	TArray<class ABaseCharacter*> BTeamChracters;		// Team == false (B팀)

	UPROPERTY(VisibleAnywhere, Category = "Info")
	TSubclassOf<class UI_Loding> LodingWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Info")
	class UI_Loding* LodingWidget;
};



