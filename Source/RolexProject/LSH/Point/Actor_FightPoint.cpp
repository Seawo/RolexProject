// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_FightPoint.h"

#include "BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "RolexPlayerState.h"


// Sets default values
AActor_FightPoint::AActor_FightPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActor_FightPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActor_FightPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 게임이 끝났거나, 거점이 비활성화라면 리턴해줘
	if (Finish != EFinish::None or ActivePoint == EActivePoint::Deactivate) return;
	DrawDebugS(DeltaTime);

}

void AActor_FightPoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void AActor_FightPoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Finish != EFinish::None or ActivePoint == EActivePoint::Deactivate) return;

	//Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UE_LOG(LogTemp, Warning, TEXT("[FightPoint] OnOverlapBegin"));

	ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);
	if (character)
	{
		bIsOverlap = true;

		UE_LOG(LogTemp, Warning, TEXT("[FightPoint] OnOverlapBegin Character : %s"), *OtherActor->GetName());
		UE_LOG(LogTemp, Warning, TEXT("[FightPoint] OnOverlapBegin RolexPS Team : %s"), character->Data.Team ? TEXT("ATeam") : TEXT("BTeam"));
		OnPointOverlapChanged.Broadcast(character->Data.Team, 1);
	}
}

void AActor_FightPoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Finish != EFinish::None or ActivePoint == EActivePoint::Deactivate) return;

	if (not bIsOverlap) return;


	//Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	UE_LOG(LogTemp, Warning, TEXT("[FightPoint] OnOverlapEnd"));

	ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);
	if (character)
	{
		bIsOverlap = false;
		OnPointOverlapChanged.Broadcast(character->Data.Team, -1);
	}
}

void AActor_FightPoint::DrawDebugS(float DeltaTime)
{
	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 0, 500), FString::Printf(TEXT("ActivePoint : %d"), ActivePoint), nullptr, FColor::Yellow, DeltaTime);
}