// Fill out your copyright notice in the Description page of Project Settings.


#include "GI.h"
#include "SaveGameEscape.h"
#include "SaveGameSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetInternationalizationLibrary.h"


void UGI::Init()
{
    Super::Init();

	LoadSettings();
}

void UGI::SaveGame()
{
	USaveGameEscape* saveGameInstance;
	if (UGameplayStatics::DoesSaveGameExist(currentSlot, 0)) {
		saveGameInstance = Cast<USaveGameEscape>(UGameplayStatics::LoadGameFromSlot(currentSlot, 0));
	}
	else {
		saveGameInstance = Cast<USaveGameEscape>(UGameplayStatics::CreateSaveGameObject(USaveGameEscape::StaticClass()));
	}
	if (saveGameInstance != nullptr) {
		saveGameInstance->levelsPlayed = levelsPlayed;
		saveGameInstance->currentLevel = currentLevel;
		saveGameInstance->playersInfo = playersInfo;
		saveGameInstance->numberOfDeathCharacters = numberOfDeathCharacters;
		UGameplayStatics::SaveGameToSlot(saveGameInstance, currentSlot, 0);
	}
}

void UGI::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(currentSlot, 0)) {
		USaveGameEscape* saveGameInstance = Cast<USaveGameEscape>(UGameplayStatics::LoadGameFromSlot(currentSlot, 0));
		if (saveGameInstance != nullptr) {
			levelsPlayed = saveGameInstance->levelsPlayed;
			currentLevel = saveGameInstance->currentLevel;
			playersInfo = saveGameInstance->playersInfo;
			numberOfDeathCharacters = saveGameInstance->numberOfDeathCharacters;
		}
	}
}

void UGI::SaveSettings()
{
	USaveGameSettings* saveGameInstance;
	if (UGameplayStatics::DoesSaveGameExist(settingsSlotName, 0)) {
		saveGameInstance = Cast<USaveGameSettings>(UGameplayStatics::LoadGameFromSlot(settingsSlotName, 0));
	}
	else {
		saveGameInstance = Cast<USaveGameSettings>(UGameplayStatics::CreateSaveGameObject(USaveGameSettings::StaticClass()));
	}
	if (saveGameInstance != nullptr) {
		saveGameInstance->currentSettings = currentSettings;
		UGameplayStatics::SaveGameToSlot(saveGameInstance, settingsSlotName, 0);
	}
}

void UGI::LoadSettings()
{
	if (UGameplayStatics::DoesSaveGameExist(settingsSlotName, 0)) {
		USaveGameSettings* saveGameInstance = Cast<USaveGameSettings>(UGameplayStatics::LoadGameFromSlot(settingsSlotName, 0));
		if (saveGameInstance != nullptr) {
			currentSettings = saveGameInstance->currentSettings;
			ApplyLanguage();
			ApplyFPS();
			ApplyResolution();
			ApplyVolume();
		}
	}
}

void UGI::ApplyLanguage()
{
	UKismetInternationalizationLibrary::SetCurrentCulture(languages[currentSettings.languageIndex], false);
}

void UGI::ApplyFPS()
{
	IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("t.MaxFPS"));
	CVar->Set(commandsFPS[currentSettings.fpsIndex]);
}

void UGI::ApplyResolution()
{
	IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
	CVar->Set(commandsResolution[currentSettings.resolutionIndex]);
}

void UGI::ApplyVolume()
{	
	/*A probar
	FAudioDevice* Device = GEngine->GetAudioDevice();
	if (!Device)
	{
		return;
	}

	for (TMap<USoundClass*, FSoundClassProperties>::TIterator It(Device->SoundClasses); It; ++It)
	{
		USoundClass* SoundClass = It.Key();
		if (SoundClass)
		{
			SoundClass->Properties.Volume = currentSettings.volume;
		}
	}
	*/
}

void UGI::RestartCurrentLevel()
{
	numberOfDeathCharacters = 0;
	if (currentLevel == ScenarioName::FUBAR) {
		UGameplayStatics::OpenLevel(this, "FUBAR", true);
	}
	else if (currentLevel == ScenarioName::AFRIEND) {
		UGameplayStatics::OpenLevel(this, "AFriendWillBleed", true);
	}
	else if (currentLevel == ScenarioName::STASH) {
		UGameplayStatics::OpenLevel(this, "Stash", true);
	}
	else if (currentLevel == ScenarioName::WAKEUP) {
		UGameplayStatics::OpenLevel(this, "WakeUp", true);
	}
	else {
		UGameplayStatics::OpenLevel(this, "MoveAlong", true);
	}
}

void UGI::ResetSaveGameAttributes()
{
	levelsPlayed.Empty();
	playersInfo.Empty(2);
	numberOfDeathCharacters = 0;
}
