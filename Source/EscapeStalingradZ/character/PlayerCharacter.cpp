// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetPreferredWeaponByCharacter();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::SetPreferredWeaponByCharacter()
{
	if (characterChosen == AvailableCharacter::Becker) {
		PreferredWeapon = Weapon::MP40;
	} 
	else if(characterChosen == AvailableCharacter::Dasha) {
		PreferredWeapon = Weapon::Nagant;
	}
	else if (characterChosen == AvailableCharacter::Katyusha) {
		PreferredWeapon = Weapon::MG34;
	}
	else {
		PreferredWeapon = Weapon::Axe;
	}
}

