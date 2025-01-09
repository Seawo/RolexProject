// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_Effect.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Character_Phase.h"

#include "Net/UnrealNetwork.h"
// Sets default values
AActor_Effect::AActor_Effect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColComp = CreateDefaultSubobject<USphereComponent>(TEXT("ColComp"));
	SetRootComponent(ColComp);


	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(ColComp);

	bReplicates = true;

}

// Called when the game starts or when spawned
void AActor_Effect::BeginPlay()
{
	Super::BeginPlay();




	// 나중에 변경해야할수도있음
	Phase = Cast<ACharacter_Phase>(GetOwner());

	if (NiagaraComponent)
	{
		NiagaraComponent->Activate();
	}
}

// Called every frame
void AActor_Effect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateLocation(DeltaTime);
}