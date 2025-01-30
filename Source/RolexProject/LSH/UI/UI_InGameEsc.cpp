// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/UI/UI_InGameEsc.h"

#include "Components/Button.h"
#include "RolexPlayerController.h"
#include "Kismet\KismetSystemLibrary.h"


void UUI_InGameEsc::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_GamePlay->OnClicked.AddDynamic(this, &UUI_InGameEsc::OnBtnGamePlayClicked);
	Btn_Option->OnClicked.AddDynamic(this, &UUI_InGameEsc::OnBtnOptionClicked);
	Btn_GameLeave->OnClicked.AddDynamic(this, &UUI_InGameEsc::OnBtnGameLeaveClicked);
	Btn_GameOver->OnClicked.AddDynamic(this, &UUI_InGameEsc::OnBtnGameOverClicked);

}

void UUI_InGameEsc::OnBtnGamePlayClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnBtnGamePlayClicked"));

	ARolexPlayerController* PC = Cast<ARolexPlayerController>(GetOwningPlayer());
	if (PC)
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
	}
	RemoveFromParent();
}

void UUI_InGameEsc::OnBtnOptionClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnBtnOptionClicked"));

	// 옵션창 띄우기
}

void UUI_InGameEsc::OnBtnGameLeaveClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnBtnGameLeaveClicked"));

	// 세션 나가기
}

void UUI_InGameEsc::OnBtnGameOverClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnBtnGameOverClicked"));

	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}