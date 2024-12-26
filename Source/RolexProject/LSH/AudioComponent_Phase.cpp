// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/AudioComponent_Phase.h"

UAudioComponent_Phase::UAudioComponent_Phase()
{
}

void UAudioComponent_Phase::PlaySound(FName Key, float InPlayRate, FName StartSectionName)
{
	if (Sounds.Contains(Key))
	{
		SetSound(Sounds[Key]);
		SetVolumeMultiplier(MasterVolume);
		SetPitchMultiplier(MasterPitch);
		Play(0.0f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Sound Key is not exist"));
	}
}