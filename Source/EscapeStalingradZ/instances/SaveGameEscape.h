// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GI.h"
#include "SaveGameEscape.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API USaveGameEscape : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY() TArray<TEnumAsByte<ScenarioName>> levelsPlayed;

	UPROPERTY() TEnumAsByte<ScenarioName> currentLevel;

	UPROPERTY() TMap<int, FPlayerInfoSaved> playersInfo;

	UPROPERTY() int numberOfDeathCharacters = 0;
};
