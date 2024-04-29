// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EscapeStalingradZ/enum/Enumerates.h"
#include "GI.generated.h"


USTRUCT(BlueprintType)
struct FPlayerInfoSaved
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerInfo) TEnumAsByte<AvailableCharacter> characterChosen;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerInfo) int health = 4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerInfo) int exp = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerInfo) int ammo = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerInfo) int food = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerInfo) int medkit = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerInfo) TEnumAsByte<EWeapon> weapon1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerInfo) TEnumAsByte<EWeapon> weapon2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerInfo) TEnumAsByte<EWeapon> weapon3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerInfo) TEnumAsByte<EWeapon> weapon4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerInfo) TEnumAsByte<EWeapon> readyWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerInfo) TEnumAsByte<EWeapon> secondaryWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerInfo) bool useReadyWeapon = true;

	FPlayerInfoSaved()
	{
	};

	FPlayerInfoSaved(AvailableCharacter characterChosen, int health, int exp, int ammo, int food, int medkit, bool useReadyWeapon)
	{
		this->characterChosen = characterChosen;
		this->health = health;
		this->exp = exp;
		this->ammo = ammo;
		this->food = food;
		this->medkit = medkit;
		this->useReadyWeapon = useReadyWeapon;
	}

	void SetWeapons(EWeapon w1, EWeapon w2, EWeapon w3, EWeapon w4, EWeapon rW, EWeapon sW)
	{
		this->weapon1 = w1;
		this->weapon2 = w2;
		this->weapon3 = w3;
		this->weapon4 = w4;
		this->readyWeapon = rW;
		this->secondaryWeapon = sW;
	}
};

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UGI : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	UPROPERTY(VisibleAnywhere) TArray<TEnumAsByte<ScenarioName>> levelsPlayed;

	UPROPERTY(VisibleAnywhere) TEnumAsByte<ScenarioName> currentLevel;

	UPROPERTY(VisibleAnywhere) TMap<int, FPlayerInfoSaved> playersInfo;

	UPROPERTY(VisibleAnywhere) FString currentSlot;

	UPROPERTY(VisibleAnywhere) int numberOfDeathCharacters = 0;

	UFUNCTION(BlueprintCallable) void SaveGame();  //desde el slot guardado
	UFUNCTION(BlueprintCallable) void LoadGame();
	UFUNCTION() void RestartCurrentLevel();
	
};
