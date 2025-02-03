// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectSpawnNotify.h"
#include "EffectActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

#include "Character_Rampage.h"

void UEffectSpawnNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp || !EffectActorClass)
    {
        return;
    }

    FVector spawnLocation = MeshComp->GetComponentLocation() + MeshComp->GetForwardVector()* LocationOffset.X + MeshComp->GetRightVector()*LocationOffset.Y + MeshComp->GetUpVector() * LocationOffset.Z;

    FRotator SpawnRotation = MeshComp->GetComponentRotation() + RotationOffset;

    //AEffectActor* spawnActor = MeshComp->GetWorld()->SpawnActor<AEffectActor>(EffectActorClass, spawnLocation, SpawnRotation);

    AEffectActor* spawnActor = MeshComp->GetWorld()->SpawnActorDeferred<AEffectActor>(EffectActorClass, FTransform(SpawnRotation, spawnLocation), MeshComp->GetOwner());

    if (spawnActor)
    {
        USphereComponent* sphereComp = spawnActor->FindComponentByClass<USphereComponent>();

        sphereComp->SetSphereRadius(collionRang);

        spawnActor->FinishSpawning(FTransform(SpawnRotation, spawnLocation));

        if (NiagaraEffect)
        {
            spawnActor->InitializeEffect(NiagaraEffect, EffectScale);
        }
        else if (ParticleEffect)
        {
            spawnActor->InitializeEffect(ParticleEffect, EffectScale);
        }
    }
}
