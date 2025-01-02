// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Actor/Actor_Effect_Muriel_Orb.h"

#include "Components/SphereComponent.h"
#include "Character_Phase.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Character_Muriel.h"
#include "Camera/CameraComponent.h"

AActor_Effect_Muriel_Orb::AActor_Effect_Muriel_Orb()
{
	PrimaryActorTick.bCanEverTick = true;

	OrbCollision = CreateDefaultSubobject<USphereComponent>(TEXT("OrbCollision"));
	OrbCollision->SetupAttachment(RootComponent);

	Speed = 1000.0f;
}

void AActor_Effect_Muriel_Orb::BeginPlay()
{
	Super::BeginPlay();

	Muriel = Cast<ACharacter_Muriel>(GetOwner());

	OrbCollision->OnComponentBeginOverlap.AddDynamic(this, &AActor_Effect_Muriel_Orb::OnOverlapBegin);

	OrbScale = 0.3f;
	SetActorScale3D(FVector(OrbScale));


	UE_LOG(LogTemp, Log, TEXT("bIsLMB : %d"), bIsLMB);
	if (bIsLMB)
	{
		SetLifeSpan(3.0f);
	}
	else
	{
		SetLifeSpan(6.0f);
	}

	UE_LOG(LogTemp, Log, TEXT("[Orb] GetActorForwardVector : %s"), *GetActorForwardVector().ToString());
}

void AActor_Effect_Muriel_Orb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Muriel->GetIsRMBCharging())
	{
		if (bIsFire) return;

		UpdateScale(DeltaTime);

		//return;
	}
	else
	{
		if (not bIsGetRotator)
		{
			UpdateRotation();
			bIsGetRotator = true;
		}

		bIsFire = true;
		UpdateLocation(DeltaTime);
	}
}

void AActor_Effect_Muriel_Orb::UpdateLocation(float DeltaTime)
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

void AActor_Effect_Muriel_Orb::UpdateScale(float DeltaTime)
{
	FVector newLoc = Muriel->GetMesh()->GetSocketLocation("Muzzle_01");

	SetActorLocation(newLoc + FVector(0, -0.3f * DeltaTime, 0));
	//SetActorRotation(Muriel->TpsCamComp->GetForwardVector().Rotation());
	SetActorRotation(FRotator(GetActorForwardVector().Rotation().Pitch, Muriel->TpsCamComp->GetForwardVector().Rotation().Yaw, Muriel->TpsCamComp->GetForwardVector().Rotation().Roll));

	if (NiagaraComponent)
	{
		OrbScale +=  0.3f * DeltaTime;
		if (OrbScale >= 1.0f)
		{
			OrbScale = 1.0f;
		}

		SetActorScale3D(FVector(OrbScale));
	}
}

void AActor_Effect_Muriel_Orb::UpdateRotation()
{
	/** 에임 방향으로 Orb 날리기*/
	FVector target;
	FRotator rot = Muriel->SetAimDirection(Muriel, target, GetActorLocation());
	SetActorRotation(rot);
}

void AActor_Effect_Muriel_Orb::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this)
	{
		//UE_LOG(LogTemp, Log, TEXT("OtherActor Equal This"));
		return;
	}
	if (OtherActor == GetOwner())
	{
		//UE_LOG(LogTemp, Log, TEXT("OtherActor Equal Owner"));
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

	//UE_LOG(LogTemp, Warning, TEXT("[Orb] Other : %s, Owner : %s"),
	//	*OtherActor->GetName(), *GetOwner()->GetName());
	//UE_LOG(LogTemp, Log, TEXT("[Orb] Overlap Begin"));
	//UE_LOG(LogTemp, Warning, TEXT("[Orb] HitLocation x : %.2f, y : %.2f, z : %.2f"), HitLocation1.X, HitLocation1.Y, HitLocation1.Z);

	ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);
	ABaseCharacter* onwer = Cast<ABaseCharacter>(GetOwner());

	if (bIsLMB)
	{
		// 캐릭터 이면서 다른 팀이라면
		if (character && character->Data.Team != onwer->Data.Team)
		{
			character->ModifyHP(-1);
		}
		else if (character && character->Data.Team == onwer->Data.Team)
		{
			character->ModifyHP(1);
		}
	}
	else
	{
		// 캐릭터 이면서 다른 팀이라면
		if (character && character->Data.Team != onwer->Data.Team)
		{
			character->ModifyHP(-1 * 10 * GetActorScale3D().X);
		}
		else if (character && character->Data.Team == onwer->Data.Team)
		{
			character->ModifyHP(1 * 10 * GetActorScale3D().X);
		}
	}


	Destroy();
}


