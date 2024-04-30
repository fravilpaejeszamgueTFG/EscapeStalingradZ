// Fill out your copyright notice in the Description page of Project Settings.


#include "WOptionsMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "EscapeStalingradZ/instances/GI.h"


UWOptionsMenu::UWOptionsMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWOptionsMenu::NativeConstruct()
{
	Super::NativeConstruct();

	Resume->OnClicked.AddDynamic(this, &UWOptionsMenu::OnClickResume);
	RestartLevel->OnClicked.AddDynamic(this, &UWOptionsMenu::OnClickRestartLevel);
	Settings->OnClicked.AddDynamic(this, &UWOptionsMenu::OnClickSettings);
	MainMenu->OnClicked.AddDynamic(this, &UWOptionsMenu::OnClickMainMenu);
}

void UWOptionsMenu::OnClickResume()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	SetVisibility(ESlateVisibility::Hidden);
}

void UWOptionsMenu::OnClickRestartLevel()
{
	UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		GI->RestartCurrentLevel();
	}
}

void UWOptionsMenu::OnClickSettings()
{
	
}

void UWOptionsMenu::OnClickMainMenu()
{
	UGameplayStatics::OpenLevel(this, "MainMenu", true);
}
