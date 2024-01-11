// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "EscapeStalingradZ/Grid/Grid.h"
#include "EscapeStalingradZ/zombies/Zombie.h"

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
			int d = getDistanceLoF(list, index);
			LoFs.Add(i, FTilesLoF(list, d));
		}
	}
}

int APlayerCharacter::getDistanceLoF(TArray<FIntPoint> tiles, FIntPoint index)
{
	int cont = 0;
	for (int i = 0; i < tiles.Num(); i++) {
		if (i == 0) {
			if ((tiles[i].X != index.X) && (tiles[i].Y != index.Y)) {
				cont += 2;
			}
			else {
				cont++;
			}
		}
		else {
			if ((tiles[i].X != tiles[i-1].X) && (tiles[i].Y != tiles[i-1].Y)) {
				cont += 2;
			}
			else {
				cont++;
			}
		}
	}
	return cont;
}

void APlayerCharacter::AttackZombieHandToHand(AZombie* zombie, FIntPoint tileZombie)
{
	UE_LOG(LogTemp, Warning, TEXT("Zombie: %s, posición: %s"), *zombie->GetName(), *tileZombie.ToString());
	//TO-DO se implementara en scoring a hit
}

int APlayerCharacter::GetDistanceAttackHandToHand()
{
	if (useReadyWeapon) {
		if (readyWeapon->multipleRange == 0) {
			return readyWeapon->pointBlankRange;
		}
	}
	else {
		if (readySecondaryWeapon->multipleRange == 0) {
			return readySecondaryWeapon->pointBlankRange;
		}
	}
	return 1;
}

TArray<FIntPoint> APlayerCharacter::GetIndexHandToHand2Range()
{
	TArray<FIntPoint> list = TArray<FIntPoint>();
	for (FIntPoint i : grid->GetTileNeighbors(grid->GetTileIndexFromLocation(GetActorLocation()))) {
		AZombie* zombie = Cast<AZombie>(grid->gridTiles[i].actor);
		if (zombie != nullptr) {
			list.Add(i);
		}
	}
	int cont = 2;
	if (list.Num() > 0) {
		cont = 1;
	}
	for (auto iter = LoFs.begin(); iter != LoFs.end(); ++iter) {
		if (iter.Value().distance > 0 && iter.Value().distance <= cont) {
			if (iter.Value().distance < cont) {
				list.Empty();
				list.AddUnique(iter.Key());
				cont = iter.Value().distance;
			}
			else {
				list.AddUnique(iter.Key());
			}
		}
	}
	return list;
}



