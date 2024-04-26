// Fill out your copyright notice in the Description page of Project Settings.


#include "WNewOrLoadGame.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "EscapeStalingradZ/instances/GI.h"
#include "EscapeStalingradZ/instances/SaveGameEscape.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Border.h"

UWNewOrLoadGame::UWNewOrLoadGame(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWNewOrLoadGame::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonSlot1->OnClicked.AddDynamic(this, &UWNewOrLoadGame::OnClickButtonSlot1);
	ButtonSlot2->OnClicked.AddDynamic(this, &UWNewOrLoadGame::OnClickButtonSlot2);
	ButtonSlot3->OnClicked.AddDynamic(this, &UWNewOrLoadGame::OnClickButtonSlot3);
	ButtonConfirm->OnClicked.AddDynamic(this, &UWNewOrLoadGame::OnClickButtonConfirm);
	ButtonBack->OnClicked.AddDynamic(this, &UWNewOrLoadGame::OnClickButtonBack);

	GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void UWNewOrLoadGame::OnClickButtonSlot1()
{
	if (GI != nullptr) {
		OnClickButtonGivenSlot(FString("Slot1"));
	}
}

void UWNewOrLoadGame::OnClickButtonSlot2()
{
	if (GI != nullptr) {
		OnClickButtonGivenSlot(FString("Slot2"));
	}
}

void UWNewOrLoadGame::OnClickButtonSlot3()
{
	if (GI != nullptr) {
		OnClickButtonGivenSlot(FString("Slot3"));
	}
}

void UWNewOrLoadGame::OnClickButtonGivenSlot(FString slotName)
{
	GI->currentSlot = slotName;
	if (isNewGame) {
		if (UGameplayStatics::DoesSaveGameExist(slotName, 0)) {
			confirmationText->SetText(NSLOCTEXT("Loadgame", "OverWriteSlot", "Overwrite this save slot?"));
		}
		else {
			confirmationText->SetText(NSLOCTEXT("Loadgame", "StartGame", "Start in this save slot?"));
		}
		levelToLoad = "FUBAR";
	}
	else {
		if (savedGames.Contains(slotName)) {
			confirmationText->SetText(NSLOCTEXT("Loadgame", "LoadSlot", "Load this save slot?"));
			savedGames[slotName]->currentLevel;
			SetLevelToLoadGivenScenario(savedGames[slotName]->currentLevel);
		}
	}
	ButtonSlot1->SetVisibility(ESlateVisibility::HitTestInvisible);
	ButtonSlot2->SetVisibility(ESlateVisibility::HitTestInvisible);
	ButtonSlot3->SetVisibility(ESlateVisibility::HitTestInvisible);
	borderConfirm->SetVisibility(ESlateVisibility::Visible);
}

void UWNewOrLoadGame::OnClickButtonConfirm()
{
	if (!isNewGame) {
		GI->LoadGame();
	}
	UGameplayStatics::OpenLevel(this, levelToLoad, true);
}

void UWNewOrLoadGame::OnClickButtonBack()
{
	ButtonSlot1->SetVisibility(ESlateVisibility::Visible);
	ButtonSlot2->SetVisibility(ESlateVisibility::Visible);
	ButtonSlot3->SetVisibility(ESlateVisibility::Visible);
	borderConfirm->SetVisibility(ESlateVisibility::Hidden);
}

FText UWNewOrLoadGame::GetCurrentLevelText(ScenarioName scenario)
{
	if (scenario == ScenarioName::FUBAR) {
		return NSLOCTEXT("Scenarios", "Fubar", "F.U.B.A.R");
	}
	else if (scenario == ScenarioName::AFRIEND) {
		return NSLOCTEXT("Scenarios", "AFriend", "A Friend Will Bleed");
	}
	else if (scenario == ScenarioName::STASH) {
		return NSLOCTEXT("Scenarios", "Stash", "STASH");
	}
	else if (scenario == ScenarioName::WAKEUP) {
		return NSLOCTEXT("Scenarios", "WakeUp", "Wake Up-Time to Die");
	}
	return NSLOCTEXT("Scenarios", "MoveAlong", "Move Along");
}

void UWNewOrLoadGame::SetVisibilityTextAndImages()
{
	for (int i = 1; i < 4; i++) {
		FString slotName = FString("Slot").Append(FString::FromInt(i));
		if (UGameplayStatics::DoesSaveGameExist(slotName, 0)) {
			USaveGameEscape* saveGameInstance = Cast<USaveGameEscape>(UGameplayStatics::LoadGameFromSlot(slotName, 0));
			if (saveGameInstance != nullptr) {
				savedGames.Add(slotName, saveGameInstance);
			}
		}
	}
	SetVisibilityTextAndImagesByGivenParameters(FString("Slot1"), NewGame1, ImageSlot1, currentLevel1, HorizontalBoxSaveGameSlot1);
	SetVisibilityTextAndImagesByGivenParameters(FString("Slot2"), NewGame2, ImageSlot2, CurrentLevel2, HorizontalBoxSaveGameSlot2);
	SetVisibilityTextAndImagesByGivenParameters(FString("Slot3"), NewGame3, ImageSlot3, CurrentLevel3, HorizontalBoxSaveGameSlot3);
	if (!isNewGame) {
		SetEnableButtonsInLoadGame(FString("Slot1"), ButtonSlot1);
		SetEnableButtonsInLoadGame(FString("Slot2"), ButtonSlot2);
		SetEnableButtonsInLoadGame(FString("Slot3"), ButtonSlot3);
		PrecacheMapsInLoad();
	}
}

void UWNewOrLoadGame::PrecacheGivenPackage(const FString& name)
{
	// Using LoadPackageAsync to precache the level
	LoadPackageAsync(name,
		FLoadPackageAsyncDelegate::CreateLambda([=](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
			{
				if (Result == EAsyncLoadingResult::Succeeded)
				{
					packages.Add(LoadedPackage);
				}
			}),
		0,
				PKG_ContainsMap);
}

void UWNewOrLoadGame::SetVisibilityTextAndImagesByGivenParameters(FString slotName, UTextBlock* newGame, UImage* imagen, UTextBlock* currentLevel, UHorizontalBox* box)
{
	if (savedGames.Contains(slotName)) {
		newGame->SetVisibility(ESlateVisibility::Hidden);
		USaveGameEscape* saveGameInstance = savedGames[slotName];
		SetCharacterImageGivenImageAndCharacterName(imagen, saveGameInstance->playersInfo[0].characterChosen);
		currentLevel->SetText(GetCurrentLevelText(saveGameInstance->currentLevel));
	}
	else {
		box->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWNewOrLoadGame::SetEnableButtonsInLoadGame(FString slotName, UButton* button)
{
	if (!savedGames.Contains(slotName)) {
		button->SetIsEnabled(false);
	}
}

void UWNewOrLoadGame::PrecacheMapsInLoad()
{
	for (auto& saveGame : savedGames) {
		PrecacheGivenPackage(GetRouteGivenScenarioName(saveGame.Value->currentLevel));
	}
}

FString UWNewOrLoadGame::GetRouteGivenScenarioName(ScenarioName scenario)
{
	if (scenario == ScenarioName::FUBAR) {
		return "/Game/Maps/FUBAR";
	}
	else if (scenario == ScenarioName::AFRIEND) {
		return "/Game/Maps/AFriendWillBleed";
	}
	else if (scenario == ScenarioName::STASH) {
		return "/Game/Maps/Stash";
	}
	else if (scenario == ScenarioName::WAKEUP) {
		return "/Game/Maps/WakeUp";
	}
	return "/Game/Maps/MoveAlong";
}

void UWNewOrLoadGame::SetLevelToLoadGivenScenario(ScenarioName scenario)
{
	if (scenario == ScenarioName::FUBAR) {
		levelToLoad = "FUBAR";
	}
	else if (scenario == ScenarioName::AFRIEND) {
		levelToLoad = "AFriendWillBleed";
	}
	else if (scenario == ScenarioName::STASH) {
		levelToLoad = "Stash";
	}
	else if (scenario == ScenarioName::WAKEUP) {
		levelToLoad = "WakeUp";
	}
	else {
		levelToLoad = "MoveAlong";
	}
}
