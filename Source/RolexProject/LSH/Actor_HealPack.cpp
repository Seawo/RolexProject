// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Actor_HealPack.h"

#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "Actor_HealPack.h"
#include "BaseCharacter.h"


// Sets default values
AActor_HealPack::AActor_HealPack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ColComp = CreateDefaultSubobject<UBoxComponent>(TEXT("ColComp"));
	SetRootComponent(ColComp);

	NiagaraComponentHealPack = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponentHealPack"));
	NiagaraComponentHealPack->SetupAttachment(ColComp);
}

// Called when the game starts or when spawned
void AActor_HealPack::BeginPlay()
{
	Super::BeginPlay();
	
	ColComp->OnComponentBeginOverlap.AddDynamic(this, &AActor_HealPack::OnOverlapBegin);
}

// Called every frame
void AActor_HealPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActor_HealPack::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (not bIsActivated) return;


	UE_LOG(LogTemp, Warning, TEXT("HealPack Overlap Begin"));
	ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);

	if (character)
	{
		if (character->Data.Hp == character->Data.MaxHp) return;


		UE_LOG(LogTemp, Warning, TEXT("HealPack Overlap Begin2"));

		character->ModifyHP(100);
		NiagaraComponentHealPack->SetVisibility(false);
		bIsActivated = false;
	}

	// 5초 후에 다시 활성화
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, [this]()
		{
			NiagaraComponentHealPack->SetVisibility(true);
			bIsActivated = true;
		}, 5.0f, false);
}

