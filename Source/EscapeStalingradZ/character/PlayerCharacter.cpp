// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "EscapeStalingradZ/Grid/Grid.h"
#include "EscapeStalingradZ/zombies/Zombie.h"
#include "EscapeStalingradZ/widget/WDicesCombat.h"
#include "EscapeStalingradZ/widget/WDiceSpreadCombat.h"

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
	LoFs.Empty();
	for (FIntPoint i : AoF) {
		grid->RemoveTileState(i, TileState::isInAoF);
	}
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

void APlayerCharacter::AttackZombieNormalFire(AZombie* zombie, FIntPoint tileZombie)
{
	int mod = GetNumberOfHitModifiersAttack(zombie) + GetNumberOfHitModifiersLoF(tileZombie);
	TArray<int> dice = GetDice(GetNumberOfDices());
	int finalHit = mod + GetPrimaryHitAndMultipleFire(tileZombie);
	CreateOrSetDicesCombatWidget(zombie, dice, finalHit, false, false);
}

void APlayerCharacter::AttackZombieSpreadFire(AZombie* zombie, FIntPoint tileZombie)
{
	int mod = GetNumberOfHitModifiersAttack(zombie) + GetNumberOfHitModifiersLoF(tileZombie) + 1;
	TArray<int> dice = GetDice(GetNumberOfDices());
	int finalHit = mod + GetPrimaryHitAndMultipleFire(tileZombie);
	CreateOrSetSpreadCombateWidget(zombie, dice, finalHit, false);
}

void APlayerCharacter::MoveToTileWithZombieDuringSpreadFire(AZombie* zombie, FIntPoint tileZombie)
{
	int finalHit = GetNumberOfHitModifiersAttack(zombie) + GetNumberOfHitModifiersLoF(tileZombie) 
		+ GetPrimaryHitAndMultipleFire(tileZombie) + 1;
	if (DiceSpreadCombatWidget != nullptr) {
		DiceSpreadCombatWidget->character = this;
		DiceSpreadCombatWidget->zombie = zombie;
		DiceSpreadCombatWidget->SetVisibility(ESlateVisibility::Visible);
		DiceSpreadCombatWidget->NextDie(finalHit);
	}
}

void APlayerCharacter::MoveToTileWithoutZombieDuringSpreadFire(FIntPoint destinyTile)
{
	if (DiceSpreadCombatWidget != nullptr) {
		DiceSpreadCombatWidget->tileZombie = destinyTile;
		DiceSpreadCombatWidget->RemoveDie();
	}
}

void APlayerCharacter::AttackZombieHandToHand(AZombie* zombie, FIntPoint tileZombie)
{
	int mod = GetNumberOfHitModifiersAttack(zombie) + GetNumberOfHitModifiersLoF(tileZombie);
	TArray<int> dice = GetDice(1);
	int finalHit = mod + GetPrimaryHitHandToHand();
	CreateOrSetDicesCombatWidget(zombie, dice, finalHit, true, false);
}

void APlayerCharacter::CoveringAttackHandToHand(AZombie* zombie, FIntPoint tileZombie)
{
	int mod = GetNumberOfHitModifiersAttack(zombie) + GetNumberOfHitModifiersLoF(tileZombie);
	TArray<int> dice = GetDice(1);
	int finalHit = mod + GetPrimaryHitHandToHand();
	CreateOrSetDicesCombatWidget(zombie, dice, finalHit, true, true);
}

void APlayerCharacter::CoveringAttackNormalFire(AZombie* zombie, FIntPoint tileZombie)
{
	int mod = GetNumberOfHitModifiersAttack(zombie) + GetNumberOfHitModifiersLoF(tileZombie);
	TArray<int> dice = GetDice(GetNumberOfDices());
	int finalHit = mod + GetPrimaryHitAndMultipleFire(tileZombie);
	CreateOrSetDicesCombatWidget(zombie, dice, finalHit, false, true);
}

void APlayerCharacter::CoveringAttackSpreadFire(AZombie* zombie, FIntPoint tileZombie)
{
	int mod = GetNumberOfHitModifiersAttack(zombie) + GetNumberOfHitModifiersLoF(tileZombie) + 1;
	TArray<int> dice = GetDice(GetNumberOfDices());
	int finalHit = mod + GetPrimaryHitAndMultipleFire(tileZombie);
	CreateOrSetSpreadCombateWidget(zombie, dice, finalHit, true);
	if (DiceSpreadCombatWidget != nullptr) {
		DiceSpreadCombatWidget->firstZombieIfCovering = zombie;
	}
}

TArray<int> APlayerCharacter::GetDice(int numberOfDice)
{
	TArray<int> dice = TArray<int>();
	for (int i = 1; i <= numberOfDice; i++) {
		int nextDie = FMath::RandRange(1, 12);
		dice.Add(nextDie);
	}
	return dice;
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
	FIntPoint backward = FIntPoint(0, 0);
	if (LoFs.Find(tileZombie) != nullptr) {
		for (FIntPoint index : LoFs.Find(tileZombie)->tilesLoF) {
			if (index != tileZombie && grid->gridTiles[index].actor != nullptr) {
				res += 2;
			}
			if (grid->gridTiles[index].types.Contains(TileType::Fire)) {
				res++;
			}
			if (LoFs.Find(tileZombie)->tilesLoF[0]!=index) {
				if (grid->gridTiles[index].types.Contains(TileType::Hinder)) {
					res += 2;
				}
				if (index.X != backward.X && index.Y != backward.Y) {
					FIntPoint forward = GetForwardIndexInDiagonal(index, backward); 
					FIntPoint right = GetForwardIndexInDiagonal(backward, index); 
					if (!grid->CanMoveDiagonal(index, forward, right, backward)) {
						res += 2;
					}
				}
			}
			if (res >= 4) {
				return 4;
			}
			backward = index;
		}
	}	
	return res;
}

FIntPoint APlayerCharacter::GetForwardIndexInDiagonal(FIntPoint start, FIntPoint end)
{
	return FIntPoint(end.X, start.Y);
}

int APlayerCharacter::GetNumberOfHitModifiersAttack(AZombie* zombie)
{
	int res = 0;
	if (zombie->isStunned) {
		res -= 3;
	}
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

int APlayerCharacter::GetNumberOfDices()
{
	if (useReadyWeapon) {
		return readyWeapon->rateOfFire;
	}
	else {
		return readySecondaryWeapon->rateOfFire;
	}
}

void APlayerCharacter::CreateOrSetDicesCombatWidget(AZombie* zombie, TArray<int> dice, int targetDie, bool isHandToHand, bool inCovering)
{
	if (DicesCombatWidgetClass) {
		if (DicesCombatWidget != nullptr) {
			DicesCombatWidget->character = this;
			DicesCombatWidget->zombie = zombie;
			DicesCombatWidget->SetVisibility(ESlateVisibility::Visible);
			if (inCovering) {
				DicesCombatWidget->attackInCovering = true;
			}
			else {
				DicesCombatWidget->attackInCovering = false;
			}
			DicesCombatWidget->SetDices(dice,targetDie, isHandToHand);
		}
		else {
			DicesCombatWidget = CreateWidget<UWDicesCombat>(GetWorld(), DicesCombatWidgetClass);
			if (DicesCombatWidget != nullptr) {
				DicesCombatWidget->character = this;
				DicesCombatWidget->zombie = zombie;
				DicesCombatWidget->AddToViewport();
				if (inCovering) {
					DicesCombatWidget->attackInCovering = true;
				}
				else {
					DicesCombatWidget->attackInCovering = false;
				}
				DicesCombatWidget->SetDices(dice, targetDie, isHandToHand);
			}
		}
	}
}

void APlayerCharacter::CreateOrSetSpreadCombateWidget(AZombie* zombie, TArray<int> dice, int targetDie, bool inCovering)
{
	if (DiceSpreadCombatWidgetClass) {
		if (DiceSpreadCombatWidget != nullptr) {
			DiceSpreadCombatWidget->character = this;
			DiceSpreadCombatWidget->zombie = zombie;
			DiceSpreadCombatWidget->SetVisibility(ESlateVisibility::Visible);
			if (inCovering) {
				DiceSpreadCombatWidget->attackInCovering = true;
			}
			else {
				DiceSpreadCombatWidget->attackInCovering = false;
			}
			DiceSpreadCombatWidget->SetDices(dice, targetDie);
		}
		else {
			DiceSpreadCombatWidget = CreateWidget<UWDiceSpreadCombat>(GetWorld(), DiceSpreadCombatWidgetClass);
			if (DiceSpreadCombatWidget != nullptr) {
				DiceSpreadCombatWidget->character = this;
				DiceSpreadCombatWidget->zombie = zombie;
				DiceSpreadCombatWidget->AddToViewport();
				if (inCovering) {
					DiceSpreadCombatWidget->attackInCovering = true;
				}
				else {
					DiceSpreadCombatWidget->attackInCovering = false;
				}
				DiceSpreadCombatWidget->SetDices(dice, targetDie);
			}
		}
	}
}

void APlayerCharacter::FriendlyFireGivenZombie(AZombie* zombie)
{
	FriendlyFire(grid->GetTileIndexFromLocation(zombie->GetActorLocation()));
}

FRotator APlayerCharacter::GetRotationDirectionToZombie(AZombie* zombie)
{
	FIntPoint index = grid->GetTileIndexFromLocation(GetActorLocation());
	FIntPoint zombieIndex = grid->GetTileIndexFromLocation(zombie->GetActorLocation());
	if (zombieIndex.X > index.X) {
		return FRotator(0, 0, 0);
	}
	else if (zombieIndex.X < index.X) {
		return FRotator(0, 180, 0);
	}
	else if (zombieIndex.Y > index.Y) {
		return FRotator(0, 90, 0);
	}
	else {
		return FRotator(0, 270, 0);
	}
}

void APlayerCharacter::ZombieLock(AZombie* zombie)
{
	inDirectContact = true;
	isLocked = true;
	if (zombie->mp > 0) {
		health--;
	}
	zombie->characterInContact = this;
	FRotator rotacion = GetRotationDirectionToZombie(zombie);
	SetActorRotation(rotacion);
}