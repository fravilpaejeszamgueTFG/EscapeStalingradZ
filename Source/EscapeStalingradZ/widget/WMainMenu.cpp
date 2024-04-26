// Fill out your copyright notice in the Description page of Project Settings.


#include "WMainMenu.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "WNewOrLoadGame.h"

UWMainMenu::UWMainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonNewGame->OnClicked.AddDynamic(this, &UWMainMenu::OnClickButtonNewGame);
	ButtonLoadGame->OnClicked.AddDynamic(this, &UWMainMenu::OnClickButtonLoadGame);
	ButtonSettings->OnClicked.AddDynamic(this, &UWMainMenu::OnClickButtonSettings);
	ButtonRules->OnClicked.AddDynamic(this, &UWMainMenu::OnClickButtonRules);
	ButtonExit->OnClicked.AddDynamic(this, &UWMainMenu::OnClickButtonExit);

	ButtonLoadGame->SetIsEnabled(false);
	EnableLoadButton();
}

void UWMainMenu::OnClickButtonNewGame()
{
	if (NewOrLoadGameWidgetClass) {
		UWNewOrLoadGame* newGame = CreateWidget<UWNewOrLoadGame>(GetWorld(), NewOrLoadGameWidgetClass);
		if (newGame != nullptr) {
			newGame->isNewGame = true;
			newGame->SetVisibilityTextAndImages();
			newGame->PrecacheGivenPackage("/Game/Maps/FUBAR");
			newGame->AddToViewport();
		}
	}
}

void UWMainMenu::OnClickButtonLoadGame()
{
	if (NewOrLoadGameWidgetClass) {
		UWNewOrLoadGame* newGame = CreateWidget<UWNewOrLoadGame>(GetWorld(), NewOrLoadGameWidgetClass);
		if (newGame != nullptr) {
			newGame->isNewGame = false;
			newGame->SetVisibilityTextAndImages();
			newGame->AddToViewport();
		}
	}
}

void UWMainMenu::OnClickButtonSettings()
{
	
}

void UWMainMenu::OnClickButtonRules()
{
	
}

void UWMainMenu::OnClickButtonExit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),
		UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Type::Quit, false);
}

void UWMainMenu::EnableLoadButton()
{
	for (int i = 1; i < 4; i++) {
		FString slotName = FString("Slot").Append(FString::FromInt(i));
		if (UGameplayStatics::DoesSaveGameExist(slotName, 0)) {
			ButtonLoadGame->SetIsEnabled(true);
			break;
		}
	}
}
