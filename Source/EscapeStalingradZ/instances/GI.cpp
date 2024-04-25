// Fill out your copyright notice in the Description page of Project Settings.


#include "GI.h"
#include "SaveGameEscape.h"
#include "Kismet/GameplayStatics.h"


void UGI::Init()
{
    Super::Init();

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
		}
	}
}
