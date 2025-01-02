// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Actor/MyActor_Effect_Muriel_E.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

AMyActor_Effect_Muriel_E::AMyActor_Effect_Muriel_E()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	HitNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HitNiagaraComponent"));
	HitNiagaraComponent->SetupAttachment(BoxCollision);
}

void AMyActor_Effect_Muriel_E::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AMyActor_Effect_Muriel_E::OnOverlapBegin);

	if (HitNiagaraComponent)
	{
		HitNiagaraComponent->Activate();
	}

	SetLifeSpan(3.0f);
}

void AMyActor_Effect_Muriel_E::Tick(float DeltaTime)
{
}

void AMyActor_Effect_Muriel_E::UpdateLocation(float DeltaTime)
{
}

void AMyActor_Effect_Muriel_E::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}
