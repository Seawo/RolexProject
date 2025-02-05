// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionUI.h"

#include "AudioManager.h"
#include "Components/AudioComponent.h"
#include "Components/Slider.h"
#include "Kismet/GameplayStatics.h"

void UOptionUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (AudioVolumeSlider)
	{
		AudioVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionUI::OnAudioVolumeChanged);
	}
}

void UOptionUI::OnAudioVolumeChanged(float Value)
{
	AudioManager = Cast<AAudioManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AAudioManager::StaticClass()));
	if (AudioManager)
	{
		AudioManager->AudioComponent->SetVolumeMultiplier(Value);
	}
}
