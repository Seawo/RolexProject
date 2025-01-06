// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Actor/Actor_Effect_Muriel_E.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

AActor_Effect_Muriel_E::AActor_Effect_Muriel_E()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	HitNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HitNiagaraComponent"));
	HitNiagaraComponent->SetupAttachment(BoxCollision);
}

void AActor_Effect_Muriel_E::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AActor_Effect_Muriel_E::OnOverlapBegin);

	if (HitNiagaraComponent)
	{
		HitNiagaraComponent->Activate();
	}

	SetLifeSpan(3.0f);
}

void AActor_Effect_Muriel_E::Tick(float DeltaTime)
{
}

void AActor_Effect_Muriel_E::UpdateLocation(float DeltaTime)
{
}

void AActor_Effect_Muriel_E::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}
