// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Actor/Actor_Effect_E.h"
#include "Components/SphereComponent.h"

#include "Character_Phase.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"

AActor_Effect_E::AActor_Effect_E()
{
	PrimaryActorTick.bCanEverTick = true;

	ShieldCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ShieldCollision"));
	ShieldCollision->SetupAttachment(RootComponent);

	ShieldCollision->SetRelativeScale3D(FVector(4.8f));
}

void AActor_Effect_E::BeginPlay()
{
	Super::BeginPlay();

	ShieldCollision->OnComponentBeginOverlap.AddDynamic(this, &AActor_Effect_E::OnOverlapBegin);

	FTimerHandle deathTimer;
	GetWorld()->GetTimerManager().SetTimer(deathTimer,
			FTimerDelegate::CreateLambda([this]() {Destroy(); NiagaraComponent->Deactivate(); }), Phase->ESkillDuration, false);
}

void AActor_Effect_E::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActor_Effect_E::UpdateLocation(float DeltaTime)
{
	SetActorLocation(Phase->GetActorLocation());
	SetActorRotation(Phase->TpsCamComp->GetForwardVector().Rotation());
}

void AActor_Effect_E::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

	if (bIsOverlap) return;
	bIsOverlap = true;


	UE_LOG(LogTemp, Warning, TEXT("[E] Other : %s, Owner : %s"),
		*OtherActor->GetName(), *GetOwner()->GetName());
	ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);
	if (character and OtherActor != this)
	{
		UE_LOG(LogTemp, Log, TEXT("Shield Overlap Begin"));


		FVector myLoc = GetActorLocation();
		FVector targetLoc = character->GetActorLocation();
		FVector dir = (targetLoc - myLoc).GetSafeNormal();

		// 힘적용
		character->LaunchCharacter(dir * 1000, true, true);
	}
}
