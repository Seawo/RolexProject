// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectActor.h"
#include "Components/SphereComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Kismet/GameplayStatics.h"

#include "BaseCharacter.h"

#include "Net/UnrealNetwork.h"

#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "RolexPlayerState.h"

// Sets default values
AEffectActor::AEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 충돌 컴포넌트 생성
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;
	CollisionComp->InitSphereRadius(CollisionRadius); // 범위 조정
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	NiagaraComp->SetupAttachment(RootComponent);

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->SetupAttachment(RootComponent);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(RootComponent);
	StaticMeshComp->SetSimulatePhysics(true);

	bReplicates = true;
	
	
}

// Called when the game starts or when spawned
void AEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetReplicates(true);
	SetReplicatingMovement(true);
	PlayEffectSound(SpawnSound);
	

	if (bIsDamage)
	{
		CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEffectActor::OnOverlapBegin);
	}
	

	if (bIsContinuousDamage)
	{
		GetWorld()->GetTimerManager().SetTimer(ContinuousDamageTimerHandle, this, &AEffectActor::CheckOverlapAndApplyDamage, 0.5f, true);
	}

	if (bIsShield)
	{
		CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEffectActor::ShieldOverlapBegin);

		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
	}
	else
	{
		SetLifeSpan(DestroyTime);
	}

}

// Called every frame
void AEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MoveDir.IsNearlyZero())
	{
		FVector NewVector = GetActorLocation() + (MoveDir * ThrowSpeed * DeltaTime);
		SetActorLocation(NewVector);  // 서버에서 위치 변경
	}
}

void AEffectActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


}

// 생성될때,
void AEffectActor::InitializeEffect(UNiagaraSystem* niagaraSystem, FVector scale)
{
	if (niagaraSystem)
	{
		NiagaraComp->SetAsset(niagaraSystem);
		NiagaraComp->SetWorldScale3D(scale);
		NiagaraComp->Activate(true);
	}
}

void AEffectActor::InitializeEffect(UParticleSystem* particleEffect, FVector effectScale)
{
	if (particleEffect)
	{
		ParticleComp->SetTemplate(particleEffect);
		ParticleComp->SetWorldScale3D(effectScale);
		ParticleComp->Activate(true);
	}
}

void AEffectActor::InititalizeThrowStone(const FVector& dir, float speed)
{
	// 던질때 크기 조절??
	//CollisionComp->InitSphereRadius(500.0f);
	//StaticMeshComp->SetWorldScale3D(FVector(30.0f));

	MoveDir = dir.GetSafeNormal();
	ThrowSpeed = speed;
}

void AEffectActor::PlayEffectSound(USoundWave* sound)
{
	if (AttenuationSetting && sound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			sound,
			GetActorLocation(),
			1.0f,
			1.0f,
			0.0f,
			AttenuationSetting
		);
	}
}

// 충돌 났을때
void AEffectActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner()|| OtherActor->IsA(AEffectActor::StaticClass()))
	{
		return;
	}

	if (OtherActor && OtherActor != this)
	{
		// 효과 추가: 나이아가라 변경, 파티클 추가 등
		UE_LOG(LogTemp, Log, TEXT("Effect collided with: %s"), *OtherActor->GetName());

		ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);
		ABaseCharacter* onwer = Cast<ABaseCharacter>(GetOwner());


		if (character)
		{
			if (character->Data.Team == onwer->Data.Team || character->MoveState == EMoveState::Die)
				return;

			character->ModifyHP(-Damage);
			
			UE_LOG(LogTemp, Log, TEXT("character : %s Hp : %d"), *OtherActor->GetName(), character->Data.Hp);

			if (NiagaraCollusionEffect)
			{

			}
			else if (ParticleCollusionEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					ParticleCollusionEffect,
					OtherActor->GetActorLocation(),
					FRotator::ZeroRotator,
					true // 자동 크기 조정
				);

				PlayEffectSound(CollisionSound);
			}

			FTimerHandle handle;

			GetWorld()->GetTimerManager().SetTimer(handle, FTimerDelegate::CreateLambda(
				[this]()
				{
					Destroy();
				}),
				0.2f, false);
		}
	}
}

void AEffectActor::CheckOverlapAndApplyDamage()
{
	TArray<AActor*> OverlappingActors;
	CollisionComp->GetOverlappingActors(OverlappingActors);

	// 오버랩된 첫 번째 액터에 대해서 로그 찍기
	if (OverlappingActors.Num() > 0)
	{
		if (!OverlappingActors[0] || OverlappingActors[0] == this || OverlappingActors[0] == GetOwner() || OverlappingActors[0]->IsA(AEffectActor::StaticClass()))
		{
			return;
		}

		// 첫 번째 오버랩된 액터 로그
		UE_LOG(LogTemp, Log, TEXT("Overlapping Actor: %s"), *OverlappingActors[0]->GetName());
	}

	for (AActor* Actor : OverlappingActors)
	{
		if (!Actor || Actor == this || Actor == GetOwner() || Actor->IsA(AEffectActor::StaticClass()))
		{
			return;
		}



		if (Actor && Actor->CanBeDamaged())
		{
			UGameplayStatics::ApplyDamage(Actor, Damage, GetInstigatorController(), this, nullptr);
			
			ABaseCharacter* character = Cast<ABaseCharacter>(Actor);
			ABaseCharacter* onwer = Cast<ABaseCharacter>(GetOwner());

			if (character->Data.Team == onwer->Data.Team)
				return;

			if (character)
			{
				if (NiagaraCollusionEffect)
				{

				}
				else if (ParticleCollusionEffect)
				{
					UGameplayStatics::SpawnEmitterAtLocation(
						GetWorld(),
						ParticleCollusionEffect,
						character->GetActorLocation(),
						FRotator::ZeroRotator,
						true // 자동 크기 조정
					);
				}
				
				character->ModifyHP(-Damage);
			}

		}
	}
}

void AEffectActor::ShieldOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner() || OtherActor->IsA(AEffectActor::StaticClass()))
	{
		return;
	}


	if (OtherActor && OtherActor != this)
	{
		// 효과 추가: 나이아가라 변경, 파티클 추가 등
		
		ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);
		ABaseCharacter* onwer = Cast<ABaseCharacter>(GetOwner());


		if (character)
		{
			if (character->Data.Team == onwer->Data.Team || character->MoveState == EMoveState::Die)
				return;

			UE_LOG(LogTemp, Log, TEXT("character : %s Hp : %d"), *OtherActor->GetName(), character->Data.Hp);
		}
		else
		{
			// character가 아닌다른 무언가
			UE_LOG(LogTemp, Log, TEXT("Effect collided with: %s"), *OtherActor->GetName());

			if (NiagaraCollusionEffect)
			{

			}
			else if (ParticleCollusionEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					ParticleCollusionEffect,
					OtherActor->GetActorLocation(),
					FRotator::ZeroRotator,
					true // 자동 크기 조정
				);
			}


		}
	}
}



