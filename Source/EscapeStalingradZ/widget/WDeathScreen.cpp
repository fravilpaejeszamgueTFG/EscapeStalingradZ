// Fill out your copyright notice in the Description page of Project Settings.


#include "WDeathScreen.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "EscapeStalingradZ/instances/GI.h"


UWDeathScreen::UWDeathScreen(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWDeathScreen::NativeConstruct()
{
	Super::NativeConstruct();

	buttonRestart->OnClicked.AddDynamic(this, &UWDeathScreen::OnClickRestart);
	buttonMainMenu->OnClicked.AddDynamic(this, &UWDeathScreen::OnClickMainMenu);
}

void UWDeathScreen::OnClickRestart()
{
	UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		GI->RestartCurrentLevel();
	}
}

void UWDeathScreen::OnClickMainMenu()
{
	UGameplayStatics::OpenLevel(this, "MainMenu", true);
}
