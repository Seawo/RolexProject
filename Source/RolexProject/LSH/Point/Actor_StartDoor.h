// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor_StartDoor.generated.h"

UENUM(BlueprintType)
enum class EGamePlayState : uint8
{
	None,
	Start,
	End,
};


UCLASS()
class ROLEXPROJECT_API AActor_StartDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_StartDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	void SetGamePlayState(EGamePlayState state);


public:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh;


	UPROPERTY(EditAnywhere)
	bool bTeam = false; // true ATeam, false BTeam

	bool bGameStart = false;
	bool bGameEnd = false;

private:
	UPROPERTY(VisibleAnywhere)
	EGamePlayState GamePlayState = EGamePlayState::None;
};
