// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Actor/Actor_Effect_Q.h"
#include "Components/BoxComponent.h"

#include "Character_Phase.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"

AActor_Effect_Q::AActor_Effect_Q()
{
	PrimaryActorTick.bCanEverTick = true;

	BeamCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BeamCollision"));
	BeamCollision->SetupAttachment(RootComponent);

	BeamCollision->SetRelativeScale3D(FVector(26.5f, 2.0f, 2.0f));
}

void AActor_Effect_Q::BeginPlay()
{
	Super::BeginPlay();

	BeamCollision->OnComponentBeginOverlap.AddDynamic(this, &AActor_Effect_Q::OnOverlapBegin);

	// 각각의 생성시간에 따른 Timer 설정
	FTimerHandle deathTimer;
	GetWorld()->GetTimerManager().SetTimer(deathTimer,
			FTimerDelegate::CreateLambda([this]() {
			Destroy(); 
			NiagaraComponent->Deactivate();
			OverlappedActors.Empty();
			}), 100, false);
}

void AActor_Effect_Q::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActor_Effect_Q::UpdateLocation(float DeltaTime)
{
	FVector newLoc = Phase->GetMesh()->GetSocketLocation("FX_Hand_L4");

	SetActorLocation(newLoc);
	SetActorRotation(Phase->TpsCamComp->GetForwardVector().Rotation());
}

void AActor_Effect_Q::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner() or OtherActor == this)
	{
		return;
	}

	AActor_Effect* effect = Cast<AActor_Effect>(OtherActor);
	if (effect)
	{
		return;
	}



	if (not OverlappedActors.Contains(OtherActor))
	{
		ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);
		if (character)
		{
			OverlappedActors.Add(OtherActor);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[Q] Other : %s, Owner : %s"),
		*OtherActor->GetName(), *GetOwner()->GetName());
	UE_LOG(LogTemp, Log, TEXT("[Q] Overlap Begin"));
}
