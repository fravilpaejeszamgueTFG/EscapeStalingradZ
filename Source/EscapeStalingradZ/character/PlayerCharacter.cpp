// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "EscapeStalingradZ/Grid/Grid.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(FName("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>("InstancedMesh");
	mesh->SetupAttachment(DefaultSceneRoot);

	readyWeapon = CreateDefaultSubobject<UWeapon>("readyWeapon");
	readySecondaryWeapon = CreateDefaultSubobject<UWeapon>("readySecondaryWeapon");

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
		PreferredWeapon = EWeapon::MP40;
	} 
	else if(characterChosen == AvailableCharacter::Dasha) {
		PreferredWeapon = EWeapon::Nagant;
	}
	else if (characterChosen == AvailableCharacter::Katyusha) {
		PreferredWeapon = EWeapon::MG34;
	}
	else {
		PreferredWeapon = EWeapon::Axe;
	}
}

void APlayerCharacter::getArcOfFire()
{
	FVector forwardVector = GetActorForwardVector();
	FVector rightVector = GetActorRightVector();
	FIntPoint index = grid->GetTileIndexFromLocation(GetActorLocation());
	AoF = grid->GetTilesAoF(index, forwardVector, rightVector);
	grid->SetTilesForAttack(AoF);
	TArray<FIntPoint> listZombies = grid->GetTilesWithZombies(AoF);
	for (FIntPoint i : listZombies) {
		TArray<FIntPoint> list = grid->GetTilesLoF(index,i);
		if (list.Num() > 0) {
			LoFs.Add(i, FTilesLoF(list));
		}
	}
}

