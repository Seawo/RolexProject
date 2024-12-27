// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Actor/Actor_Effect_Orb.h"
#include "Components/SphereComponent.h"
#include "Character_Phase.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystemComponent.h"

AActor_Effect_Orb::AActor_Effect_Orb()
{
	PrimaryActorTick.bCanEverTick = true;

	OrbCollision = CreateDefaultSubobject<USphereComponent>(TEXT("OrbCollision"));
	OrbCollision->SetupAttachment(RootComponent);

	HitNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HitNiagaraComponent"));
	HitNiagaraComponent->SetupAttachment(ColComp);
}

void AActor_Effect_Orb::BeginPlay()
{
	Super::BeginPlay();


	FVector start = GetActorLocation();
	FVector end = start + GetActorForwardVector() * 100000.0f;

	// 레이캐스트를 통해 충돌체크
	FHitResult hit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(GetOwner());
	if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_Visibility, params))
	{
		HitLocation1 = hit.ImpactPoint;
	}
	UE_LOG(LogTemp, Warning, TEXT("[Orb] HitLocation x : %.2f, y : %.2f, z : %.2f"), HitLocation1.X, HitLocation1.Y, HitLocation1.Z);

	OrbCollision->OnComponentBeginOverlap.AddDynamic(this, &AActor_Effect_Orb::OnOverlapBegin);

	// 각각의 생성시간에 따른 Timer 설정
	FTimerHandle deathTimer;
	GetWorld()->GetTimerManager().SetTimer(deathTimer,
		FTimerDelegate::CreateLambda([this]() {Destroy(); NiagaraComponent->Deactivate(); }), Phase->LRSkillDuration, false);
}

void AActor_Effect_Orb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActor_Effect_Orb::UpdateLocation(float DeltaTime)
{
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime);

	if (NiagaraComponent)
	{
		FVector CurrentLocation = NiagaraComponent->GetComponentLocation();
		FVector TargetLocation = GetActorLocation();

		// 선형 보간을 사용하여 부드럽게 이동
		FVector SmoothLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 10.0f);
		NiagaraComponent->SetWorldLocation(SmoothLocation);
	}
}

void AActor_Effect_Orb::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this)
	{
		UE_LOG(LogTemp, Log, TEXT("OtherActor Equal This"));
		return;
	}
	if (OtherActor == GetOwner())
	{
		UE_LOG(LogTemp, Log, TEXT("OtherActor Equal Owner"));
		return;
	}
	AActor_Effect* effect = Cast<AActor_Effect>(OtherActor);
	if (effect)
	{
		return;
	}

	// NiagaraSystem 실행
	if (HitNiagaraSystem)
	{
		FVector hitloc = OtherActor->GetActorLocation();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitNiagaraSystem, GetActorLocation());
	}

	UE_LOG(LogTemp, Warning, TEXT("[Orb] Other : %s, Owner : %s"),
		*OtherActor->GetName(), *GetOwner()->GetName());
	UE_LOG(LogTemp, Log, TEXT("[Orb] Overlap Begin"));
	UE_LOG(LogTemp, Warning, TEXT("[Orb] HitLocation x : %.2f, y : %.2f, z : %.2f"), HitLocation1.X, HitLocation1.Y, HitLocation1.Z);

	Destroy();
}