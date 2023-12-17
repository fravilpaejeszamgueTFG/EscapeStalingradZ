// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
UWeapon::UWeapon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void UWeapon::BeginPlay()
{
	Super::BeginPlay();

	SetPropiertiesByName(weaponName);
	
}

void UWeapon::SetPropiertiesNone()
{
	weaponName = EWeapon::None;
	rateOfFire = 1;
	primaryHit = 10;
	pointBlankRange = 1;
	multipleRange = 0;
	oneHandWeapon = true;
	hand2handHit = 10;
}

void UWeapon::SetPropiertiesMP40()
{
	weaponName = EWeapon::MP40;
	rateOfFire = 2;
	primaryHit = 7;
	pointBlankRange = 3;
	multipleRange = 2;
	oneHandWeapon = false;
	hand2handHit = 9;
}

void UWeapon::SetPropiertiesNagant()
{
	weaponName = EWeapon::Nagant;
	rateOfFire = 1;
	primaryHit = 5;
	pointBlankRange = 10;
	multipleRange = 10;
	oneHandWeapon = false;
	hand2handHit = 7;
}

void UWeapon::SetPropiertiesMG34()
{
	weaponName = EWeapon::MG34;
	rateOfFire = 3;
	primaryHit = 6;
	pointBlankRange = 10;
	multipleRange = 4;
	oneHandWeapon = false;
	hand2handHit = 10;
}

void UWeapon::SetPropiertiesAxe()
{
	weaponName = EWeapon::Axe;
	rateOfFire = 1;
	primaryHit = 5;
	pointBlankRange = 2;
	multipleRange = 0;
	oneHandWeapon = false;
	hand2handHit = 5;
}

void UWeapon::SetPropiertiesKnife()
{
	weaponName = EWeapon::Knife;
	rateOfFire = 1;
	primaryHit = 4;
	pointBlankRange = 1;
	multipleRange = 0;
	oneHandWeapon = true;
	hand2handHit = 4;
}

void UWeapon::SetPropiertiesLuger()
{
	weaponName = EWeapon::Luger;
	rateOfFire = 1;
	primaryHit = 6;
	pointBlankRange = 3;
	multipleRange = 1;
	oneHandWeapon = true;
	hand2handHit = 6;
}

void UWeapon::SetPropiertiesByName(EWeapon weapon)
{
	if (weapon == EWeapon::None) {
		SetPropiertiesNone();
	}
	else if (weapon == EWeapon::Knife) {
		SetPropiertiesKnife();
	}
	else if (weapon == EWeapon::Luger) {
		SetPropiertiesLuger();
	}
	else if (weapon == EWeapon::MG34) {
		SetPropiertiesMG34();
	}
	else if (weapon == EWeapon::MP40) {
		SetPropiertiesMP40();
	}
	else if (weapon == EWeapon::Nagant) {
		SetPropiertiesNagant();
	}
	else {
		SetPropiertiesAxe();
	}
}

