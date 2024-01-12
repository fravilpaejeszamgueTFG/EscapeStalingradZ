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
	int mod = GetNumberOfHitModifiersAttack(zombie);
	int die = FMath::RandRange(1, 12);
	int finalHit = mod + GetPrimaryHitHandToHand();
	UE_LOG(LogTemp, Warning, TEXT("dado: %d, finalHit: %d, modificador: %d"), die, finalHit, mod);
	if (die >= finalHit) {
		if (die >= finalHit + 3) {
			zombie->health--;
			UE_LOG(LogTemp, Warning, TEXT("muerto"));
		}
		else {
			zombie->isStunned = true;
			UE_LOG(LogTemp, Warning, TEXT("estuneado"));
		}
	}
	if (die == 2) {
		FriendlyFire(tileZombie);
	}
	//TO-DO mejorar dados
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

int APlayerCharacter::GetNumberOfHitModifiersLoF(FIntPoint tileZombie)
{
	int res = 0;
	if (LoFs.Find(tileZombie) != nullptr) {
		for (FIntPoint index : LoFs.Find(tileZombie)->tilesLoF) {
			if (grid->gridTiles[index].actor != nullptr) {
				res += 2;
			}
		}
	}
	//TO-DO
	if (res >= 4) {
		return 4;
	}
	return res;
}

int APlayerCharacter::GetNumberOfHitModifiersAttack(AZombie* zombie)
{
	int res = 0;
	if (zombie->isStunned) {
		res -= 3;
	}
	//TO-DO spread fire
	if (typeOfCovering == CoveringType::Walked) {
		res += 2;
	}
	else if (typeOfCovering == CoveringType::Ran) {
		res += 4;
	}
	else if (typeOfCovering == CoveringType::NONE) {
		if (typeOfMovement == MovementType::Stationary) {
			res -= 2;
		}
		else if (typeOfMovement == MovementType::Running) {
			res += 2;
		}
	}
	if (!useReadyWeapon) {
		res += 2;
	}

	return res;
}

int APlayerCharacter::GetPrimaryHitHandToHand()
{
	if (useReadyWeapon) {
		return readyWeapon->hand2handHit;
	}
	else {
		return readySecondaryWeapon->hand2handHit;
	}
}

int APlayerCharacter::GetPrimaryHitAndMultipleFire(FIntPoint tileZombie)
{
	int res = 0;
	int pb = 0;
	int rm = 0;
	if (useReadyWeapon) {
		res = readyWeapon->primaryHit;
		pb = readyWeapon->pointBlankRange;
		rm = readyWeapon->multipleRange;
	}
	else {
		res = readySecondaryWeapon->primaryHit;
		pb = readySecondaryWeapon->pointBlankRange;
		rm = readySecondaryWeapon->multipleRange;
	}
	if (LoFs.Find(tileZombie) != nullptr) {
		if (LoFs.Find(tileZombie)->distance > pb) {
			for (int i = pb; i < LoFs.Find(tileZombie)->distance; i += rm) {
				res++;
			}
		}
	}
	return res;
}

void APlayerCharacter::FriendlyFire(FIntPoint tileZombie)
{
	if (LoFs.Find(tileZombie) != nullptr) {
		for (FIntPoint index : LoFs.Find(tileZombie)->tilesLoF) {
			APlayerCharacter* chara = Cast<APlayerCharacter>(grid->gridTiles[index].actor);
			if (chara != nullptr) {
				chara->health--;
				UE_LOG(LogTemp, Warning, TEXT("fuego amigo"));
				break;
			}
		}
	}
}