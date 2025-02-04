// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthbarUserWidget.h"
#include "Components/ProgressBar.h"

void UHealthbarUserWidget::SetHealthPercentage(float Percentage)
{
    if (HealthProgressBar)
    {
        HealthProgressBar->SetPercent(Percentage);
    }
}

void UHealthbarUserWidget::SetShieldPercentage(float Percentage)
{
    UE_LOG(LogTemp, Warning, TEXT("percentage : %f"), Percentage);

    if (ShieldProgressBar)
    {
        ShieldProgressBar->SetPercent(Percentage);
    }
}
