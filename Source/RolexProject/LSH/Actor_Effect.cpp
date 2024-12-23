// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Actor_Effect.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AActor_Effect::AActor_Effect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	SetRootComponent(CollisionComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CollisionComp);

	OrbNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("OrbNiagaraComponent"));
	OrbNiagaraComponent->SetupAttachment(CollisionComp);


	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NE(TEXT("/Script/Niagara.NiagaraSystem'/Game/Rolex/LSH/Blueprints/Phase/Particle/NS_MagicOrb.NS_MagicOrb'"));
	if (NE.Succeeded())
	{
		OrbNiagaraSystem = NE.Object;
	}
}

// Called when the game starts or when spawned
void AActor_Effect::BeginPlay()
{
	Super::BeginPlay();
	OrbNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), OrbNiagaraSystem, GetActorLocation());

	if (OrbNiagaraComponent)
	{
		OrbNiagaraComponent->Activate();
	}

	


	FTimerHandle deathTimer;

	GetWorld()->GetTimerManager().SetTimer(deathTimer, 
		FTimerDelegate::CreateLambda([this]() {Destroy(); OrbNiagaraComponent->Deactivate();}), 3.0f, false);
}

// Called every frame
void AActor_Effect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime);

	if (OrbNiagaraComponent)
	{
		FVector CurrentLocation = OrbNiagaraComponent->GetComponentLocation();
		FVector TargetLocation = GetActorLocation();

		// 선형 보간을 사용하여 부드럽게 이동
		FVector SmoothLocation = FMath::VInterpTo(CurrentLocation, TargetLocation,DeltaTime,10.0f);
		OrbNiagaraComponent->SetWorldLocation(SmoothLocation);
	}
}