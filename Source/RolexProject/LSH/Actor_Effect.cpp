// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Actor_Effect.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Character_Phase.h"
#include "Camera/CameraComponent.h"

// Sets default values
AActor_Effect::AActor_Effect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OrbCollision = CreateDefaultSubobject<USphereComponent>(TEXT("OrbCollision"));
	SetRootComponent(OrbCollision);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(OrbCollision);

	ShieldCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ShieldCollision"));
	ShieldCollision->SetupAttachment(OrbCollision);

	OrbNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("OrbNiagaraComponent"));
	OrbNiagaraComponent->SetupAttachment(OrbCollision);


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

	// 나중에 변경해야할수도있음
	Phase = Cast<ACharacter_Phase>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (OrbNiagaraComponent)
	{
		OrbNiagaraComponent->Activate();
	}

	


	FTimerHandle deathTimer;
	if (SkillName == "LMBRMB")
	{
		GetWorld()->GetTimerManager().SetTimer(deathTimer,
			FTimerDelegate::CreateLambda([this]() {Destroy(); OrbNiagaraComponent->Deactivate(); }), Phase->LRSkillDuration, false);
	}

	else if (SkillName == "Q")
	{
		GetWorld()->GetTimerManager().SetTimer(deathTimer,
			FTimerDelegate::CreateLambda([this]() {Destroy(); OrbNiagaraComponent->Deactivate(); }), Phase->QSkillDuration, false);
	}

	else if (SkillName == "E")
	{
		GetWorld()->GetTimerManager().SetTimer(deathTimer,
			FTimerDelegate::CreateLambda([this]() {Destroy(); OrbNiagaraComponent->Deactivate(); }), Phase->ESkillDuration, false);
	}

}

// Called every frame
void AActor_Effect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateLocation(DeltaTime);
}

void AActor_Effect::UpdateLocation(float DeltaTime)
{
	if (SkillName == "LMBRMB")
	{
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime);

		if (OrbNiagaraComponent)
		{
			FVector CurrentLocation = OrbNiagaraComponent->GetComponentLocation();
			FVector TargetLocation = GetActorLocation();

			// 선형 보간을 사용하여 부드럽게 이동
			FVector SmoothLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 10.0f);
			OrbNiagaraComponent->SetWorldLocation(SmoothLocation);
		}
	}

	else if (SkillName == "Q")
	{
		FVector newLoc = Phase->GetMesh()->GetSocketLocation("FX_Hand_L4");

		SetActorLocation(newLoc);
		SetActorRotation(Phase->TpsCamComp->GetForwardVector().Rotation());
	}

	else if (SkillName == "E")
	{
		SetActorLocation(Phase->GetActorLocation());
		SetActorRotation(Phase->TpsCamComp->GetForwardVector().Rotation());
	}
}