// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


UENUM()
enum ScenarioName
{
	FUBAR     UMETA(DisplayName = "FUBAR"),
	AFRIEND     UMETA(DisplayName = "AFRIEND"),
	WAKEUP	   UMETA(DisplayName = "WAKEUP"),
	STASH	   UMETA(DisplayName = "STASH"),
	MOVEALONG	   UMETA(DisplayName = "MOVEALONG"),
};

UENUM()
enum TileType
{
	Normal     UMETA(DisplayName = "Normal"),
	Hinder     UMETA(DisplayName = "Hinder"),
	Fire	   UMETA(DisplayName = "Fire"),
	Search	   UMETA(DisplayName = "Search"),
	Spawn	   UMETA(DisplayName = "Spawn"),
	S1		UMETA(DisplayName = "S1"),
	S2		UMETA(DisplayName = "S2"),
};

UENUM()
enum TileState
{
	Hovered     UMETA(DisplayName = "Hovered"),
	Selected	   UMETA(DisplayName = "Selected"),
	isNeighbor		UMETA(DisplayName = "isNeighbor"),
	isReachable		UMETA(DisplayName = "isReachable"),
	isInAoF			UMETA(DisplayName = "isInAoF"),
	poisoned		UMETA(DisplayName = "poisoned"),
};

UENUM()
enum ObjectName
{
	Ammo     UMETA(DisplayName = "Ammo"),
	Food     UMETA(DisplayName = "Food"),
	MedKit	   UMETA(DisplayName = "MedKit"),
	WKnife     UMETA(DisplayName = "WKnife"),
	WLuger	   UMETA(DisplayName = "WLuger"),
	SecondFavWeapon	   UMETA(DisplayName = "SecondFavWeapon"),
	FavWeapon	   UMETA(DisplayName = "FavWeapon"),
	Zombie		UMETA(DisplayName = "Zombie"),
};

UENUM()
enum AvailableCharacter
{
	Becker     UMETA(DisplayName = "Becker"),
	Dasha     UMETA(DisplayName = "Dasha"),
	Katyusha   UMETA(DisplayName = "Katyusha"),
	Til     UMETA(DisplayName = "Til"),

};

UENUM()
enum MovementType
{
	Stationary     UMETA(DisplayName = "Stationary"),
	Walking     UMETA(DisplayName = "Walking"),
	Running   UMETA(DisplayName = "Running"),

};

UENUM()
enum CoveringType
{
	NONE     UMETA(DisplayName = "NONE"),
	CStationary     UMETA(DisplayName = "CStationary"),
	Walked   UMETA(DisplayName = "Walked"),
	Ran     UMETA(DisplayName = "Ran"),

};

UENUM()
enum CoveringAttackType
{
	HandToHand     UMETA(DisplayName = "HandToHand"),
	NormalFire     UMETA(DisplayName = "NormalFire"),
	SpreadFire   UMETA(DisplayName = "SpreadFire"),

};

UENUM()
enum ZombieType
{
	Beta     UMETA(DisplayName = "Beta"),
	Armoured     UMETA(DisplayName = "Armoured"),
	Kugelfisch   UMETA(DisplayName = "Kugelfisch"),
	Alpha     UMETA(DisplayName = "Alpha"),
	BetaCharacter     UMETA(DisplayName = "BetaCharacter"),

};

UENUM()
enum EWeapon
{
	None     UMETA(DisplayName = "None"),
	MP40     UMETA(DisplayName = "MP-40"),
	Nagant   UMETA(DisplayName = "Nagant"),
	MG34     UMETA(DisplayName = "MG34"),
	Axe      UMETA(DisplayName = "Axe"),
	Knife    UMETA(DisplayName = "Knife"),
	Luger     UMETA(DisplayName = "Luger"),

};
/**
 * 
 */
class ESCAPESTALINGRADZ_API Enumerates
{
public:
	
};
