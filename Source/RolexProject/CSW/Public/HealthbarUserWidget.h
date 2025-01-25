// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthbarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API UHealthbarUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    // 체력 비율 업데이트 함수
    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetHealthPercentage(float Percentage);

    // 쉴드 비율 업데이트 함수
    UFUNCTION(BlueprintCallable, Category = "Shield")
    void SetShieldPercentage(float Percentage);

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HealthProgressBar;

protected:
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* ShieldProgressBar;
};
