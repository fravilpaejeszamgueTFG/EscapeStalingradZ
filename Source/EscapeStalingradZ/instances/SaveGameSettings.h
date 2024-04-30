// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GI.h"
#include "SaveGameSettings.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API USaveGameSettings : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere) FSettings currentSettings;
	
};
