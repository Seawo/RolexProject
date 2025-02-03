// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Actor/Actor_Effect_Phase_Orb.h"
#include "Components/SphereComponent.h"
#include "Character_Phase.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "RolexPlayerState.h"

AActor_Effect_Phase_Orb::AActor_Effect_Phase_Orb()
{
	PrimaryActorTick.bCanEverTick = true;

	OrbCollision = CreateDefaultSubobject<USphereComponent>(TEXT("OrbCollision"));
	OrbCollision->SetupAttachment(RootComponent);

	HitNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HitNiagaraComponent"));
	HitNiagaraComponent->SetupAttachment(ColComp);
}

void AActor_Effect_Phase_Orb::BeginPlay()
{
	Super::BeginPlay();

	OrbCollision->OnComponentBeginOverlap.AddDynamic(this, &AActor_Effect_Phase_Orb::OnOverlapBegin);

	// 각각의 생성시간에 따른 Timer 설정
	//SetLifeSpan(Phase->LRSkillDuration);

	//FTimerHandle deathTimer;
	//GetWorld()->GetTimerManager().SetTimer(deathTimer,
	//	FTimerDelegate::CreateLambda([this]() {NiagaraComponent->Deactivate(); Destroy(); }), Phase->LRSkillDuration, false);
}

void AActor_Effect_Phase_Orb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActor_Effect_Phase_Orb::UpdateLocation(float DeltaTime)
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

void AActor_Effect_Phase_Orb::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

	//UE_LOG(LogTemp, Warning, TEXT("[Orb] Other : %s, Owner : %s"),
	//	*OtherActor->GetName(), *GetOwner()->GetName());
	//UE_LOG(LogTemp, Log, TEXT("[Orb] Overlap Begin"));
	//UE_LOG(LogTemp, Warning, TEXT("[Orb] HitLocation x : %.2f, y : %.2f, z : %.2f"), HitLocation1.X, HitLocation1.Y, HitLocation1.Z);

	ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);
	ABaseCharacter* onwer = Cast<ABaseCharacter>(GetOwner());

	if (character && character->Data.Team == onwer->Data.Team)
	{
		// 팀이면 충돌x
		return;
	}
	// 캐릭터 이면서 다른 팀이라면
	else if (character && character->Data.Team != onwer ->Data.Team)
	{
		if (character->Data.Hp <= 0)
		{
			onwer->RolexPS->PlayerData.KillCount++;
		}
		else if (character->Data.Hp < 10)
		{
			onwer->RolexPS->PlayerData.Damage += character->Data.Hp;
			character->ModifyHP(-character->Data.Hp);
		}
		else
		{
			character->ModifyHP(-10);
			onwer->RolexPS->PlayerData.Damage += 10;
		}
	}

	// NiagaraSystem 실행
	if (HitNiagaraSystem)
	{
		FVector hitloc = OtherActor->GetActorLocation();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitNiagaraSystem, GetActorLocation());
	}

	Destroy();
}