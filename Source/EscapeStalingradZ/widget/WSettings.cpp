// Fill out your copyright notice in the Description page of Project Settings.


#include "WSettings.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "EscapeStalingradZ/instances/GI.h"
#include "Kismet/GameplayStatics.h"

UWSettings::UWSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWSettings::NativeConstruct()
{
    Super::NativeConstruct();

    ButtonSpanish->OnClicked.AddDynamic(this, &UWSettings::OnClickSpanish);
    ButtonEnglish->OnClicked.AddDynamic(this, &UWSettings::OnClickEnglish);
    ButtonLeftFPS->OnClicked.AddDynamic(this, &UWSettings::OnClickButtonLeftFPS);
    ButtonRightFPS->OnClicked.AddDynamic(this, &UWSettings::OnClickButtonRightFPS);
    ButtonLeftResolution->OnClicked.AddDynamic(this, &UWSettings::OnClickButtonLeftResolution);
    ButtonRightResolution->OnClicked.AddDynamic(this, &UWSettings::OnClickButtonRightResolution);
    ButtonBack->OnClicked.AddDynamic(this, &UWSettings::OnClickBack);

    GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void UWSettings::OnClickSpanish()
{
    if (GI != nullptr) {
        GI->currentSettings.languageIndex = 1;
        GI->ApplyLanguage();
    }
}

void UWSettings::OnClickEnglish()
{
    if (GI != nullptr) {
        GI->currentSettings.languageIndex = 0;
        GI->ApplyLanguage();
    }
}

void UWSettings::OnClickButtonLeftFPS()
{
    if (GI != nullptr) {
        GI->currentSettings.fpsIndex--;
        if (GI->currentSettings.fpsIndex <= 0) {
            ButtonLeftFPS->SetIsEnabled(false);
        }
        ButtonRightFPS->SetIsEnabled(true);
        GI->ApplyFPS();
        SetTextFPSGivenIndex(GI->currentSettings.fpsIndex);
    }
}

void UWSettings::OnClickButtonRightFPS()
{
    if (GI != nullptr) {
        GI->currentSettings.fpsIndex++;
        if (GI->currentSettings.fpsIndex >= (GI->commandsFPS.Num() - 1)) {
            ButtonRightFPS->SetIsEnabled(false);
        }
        ButtonLeftFPS->SetIsEnabled(true);
        GI->ApplyFPS();
        SetTextFPSGivenIndex(GI->currentSettings.fpsIndex);
    }
}

void UWSettings::OnClickButtonLeftResolution()
{
    if (GI != nullptr) {
        GI->currentSettings.resolutionIndex--;
        if (GI->currentSettings.resolutionIndex <= 0) {
            ButtonLeftResolution->SetIsEnabled(false);
        }
        ButtonRightResolution->SetIsEnabled(true);
        GI->ApplyResolution();
        SetTextResolutionGivenIndex(GI->currentSettings.resolutionIndex);
    }
}

void UWSettings::OnClickButtonRightResolution()
{
    if (GI != nullptr) {
        GI->currentSettings.resolutionIndex++;
        if (GI->currentSettings.resolutionIndex >= (GI->commandsResolution.Num() - 1)) {
            ButtonRightResolution->SetIsEnabled(false);
        }
        ButtonLeftResolution->SetIsEnabled(true);
        GI->ApplyResolution();
        SetTextResolutionGivenIndex(GI->currentSettings.resolutionIndex);
    }
}

void UWSettings::OnClickBack()
{
    if (GI != nullptr) {
        GI->SaveSettings();
        SetVisibility(ESlateVisibility::Hidden);
    }
}

void UWSettings::DisableButtonsInitial()
{
    if (GI != nullptr) {
        int resIndex = GI->currentSettings.resolutionIndex;
        int fIndex = GI->currentSettings.fpsIndex;
        if (resIndex >= (GI->commandsResolution.Num() - 1)) {
            ButtonRightResolution->SetIsEnabled(false);
        } else if (resIndex <= 0) {
            ButtonLeftResolution->SetIsEnabled(false);
        }
        if (fIndex >= (GI->commandsFPS.Num() - 1)) {
            ButtonRightFPS->SetIsEnabled(false);
        }
        else if (fIndex <= 0) {
            ButtonLeftFPS->SetIsEnabled(false);
        }
        SetTextFPSGivenIndex(fIndex);
        SetTextResolutionGivenIndex(resIndex);
    }
}

void UWSettings::SetTextFPSGivenIndex(int index)
{
    TextFPS->SetText(FText::FromString((fpsTexts[index])));
}

void UWSettings::SetTextResolutionGivenIndex(int index)
{
    TextResolution->SetText(resolutionsTexts[index]);
}
