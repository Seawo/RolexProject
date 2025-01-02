// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Actor/Actor_Effect_Phase_Q.h"
#include "Components/BoxComponent.h"

#include "Character_Phase.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"

AActor_Effect_Phase_Q::AActor_Effect_Phase_Q()
{
	PrimaryActorTick.bCanEverTick = true;

	BeamCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BeamCollision"));
	BeamCollision->SetupAttachment(RootComponent);

	BeamCollision->SetRelativeScale3D(FVector(26.5f, 2.0f, 2.0f));
}

void AActor_Effect_Phase_Q::BeginPlay()
{
	Super::BeginPlay();

	// 충돌체 BeginOverlap
	BeamCollision->OnComponentBeginOverlap.AddDynamic(this, &AActor_Effect_Phase_Q::OnOverlapBegin);

	// 충돌체 EndOverlap
	BeamCollision->OnComponentEndOverlap.AddDynamic(this, &AActor_Effect_Phase_Q::OnOverlapEnd);



	// 각각의 생성시간에 따른 Timer 설정
	FTimerHandle deathTimer;
	GetWorld()->GetTimerManager().SetTimer(deathTimer,
			FTimerDelegate::CreateLambda([this]() {
			Destroy(); 
			NiagaraComponent->Deactivate();
			OverlappedActors.Empty();
			}), 100, false);
		// Phase->QSkillDuration


	// 라인트레이스
	FTimerHandle lineTraceTimer;
	GetWorld()->GetTimerManager().SetTimer(lineTraceTimer, this, &AActor_Effect_Phase_Q::DrawLineTrace,0.2f,true);
}

void AActor_Effect_Phase_Q::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActor_Effect_Phase_Q::UpdateLocation(float DeltaTime)
{
	FVector newLoc = Phase->GetMesh()->GetSocketLocation("FX_Hand_L4");

	SetActorLocation(newLoc);
	SetActorRotation(Phase->TpsCamComp->GetForwardVector().Rotation());
}

void AActor_Effect_Phase_Q::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

	bIsOverlap = true;
	// 겹치는 대상 추가 및 중복추가 방지
	if (not OverlappedActors.Contains(OtherActor))
	{
		ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);
		if (character)
		{
			OverlappedActors.Add(character);
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("[Q] Other : %s, Owner : %s"),*OtherActor->GetName(), *GetOwner()->GetName());
	UE_LOG(LogTemp, Log, TEXT("[Q] Overlap Begin"));

	GetWorld()->GetTimerManager().SetTimer(DamageTimer, this, &AActor_Effect_Phase_Q::TakeDamageToCharacter, 0.2f, true);
}

void AActor_Effect_Phase_Q::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == GetOwner() or OtherActor == this)
	{
		return;
	}

	// 이펙트끼리의 충돌 무시
	AActor_Effect* effect = Cast<AActor_Effect>(OtherActor);
	if (effect)
	{
		return;
	}

	bIsOverlap = false;

	// 겹치지않은 대상 제거
	if (OverlappedActors.Contains(OtherActor))
	{
		ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);
		OverlappedActors.Remove(character);
	}

	UE_LOG(LogTemp, Log, TEXT("[Q] Overlap End"));
	
	// 타이머 해제
	GetWorld()->GetTimerManager().ClearTimer(DamageTimer);
}

void AActor_Effect_Phase_Q::TakeDamageToCharacter()
{
	// 데미지 주기
	if (bIsOverlap)
	{
		for (ABaseCharacter* character : OverlappedActors)
		{
			if (character)
			{
				character->ModifyHP(-1);
			}
		}
	}
}

void AActor_Effect_Phase_Q::DrawLineTrace()
{
	// 라인트레이스
	FVector start = GetActorLocation();
	FVector end = start + GetActorForwardVector() * 1800.0f;

	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(GetOwner());
	FVector pivot = FVector(840,0,0);


	if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, params))
	{
		//UE_LOG(LogTemp, Log, TEXT("Hit : %s"), *hitResult.GetActor()->GetName());
		ABaseCharacter* characer = Cast<ABaseCharacter>(hitResult.GetActor());
		if (characer)
		{
			LineTraceDistance = 1800.0f;
			BeamCollision->SetRelativeLocation(pivot);
		}
		else
		{
			// 충돌위치값가져오기
			FVector hitLoc = hitResult.ImpactPoint;
			LineTraceDistance = FVector::Distance(start, hitLoc);

			pivot = FMath::Lerp(GetActorLocation(), hitLoc, 0.5f);
			BeamCollision->SetWorldLocation(pivot);
		}
	}
	else
	{
		//UE_LOG(LogTemp, Log, TEXT("Hit false"));

		LineTraceDistance = 1800.0f;
		BeamCollision->SetRelativeLocation(pivot);
	}

	//UE_LOG(LogTemp, Log, TEXT("LineTraceDistance : %f"), LineTraceDistance);

	// 1 : 1650

	float NiagaraScaleX = FMath::Min(LineTraceDistance / 1650.0f, 1.0f);
	float ColScaleX = FMath::Min(LineTraceDistance / 1650.0f * 26.5f, 26.5f);

	// 라인트레이스 길이에 따라 나이아가라와 충돌체 크기 조절
	NiagaraComponent->SetRelativeScale3D(FVector(NiagaraScaleX, 1,1));
	BeamCollision->SetRelativeScale3D(FVector(ColScaleX, 2,2));
	


	//UE_LOG(LogTemp, Log, TEXT("ColScaleX : %f"), ColScaleX);
	//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.2f, 0, 1.0f);
}