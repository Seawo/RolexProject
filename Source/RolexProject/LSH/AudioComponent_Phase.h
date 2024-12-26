// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "AudioComponent_Phase.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API UAudioComponent_Phase : public UAudioComponent
{
	GENERATED_BODY()
	
	UAudioComponent_Phase();
	
	//virtual void BeginPlay() override;
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	
public:
	void PlaySound(FName Key, float InPlayRate = 1.0f, FName StartSectionName = NAME_None);




public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound_Phase")
	TMap<FName, USoundBase*> Sounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound_Phase")
	float MasterVolume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound_Phase")
	float MasterPitch = 1.0f;
};
