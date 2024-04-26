// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "EscapeStalingradZ/Grid/Grid.h"
#include "EscapeStalingradZ/zombies/Zombie.h"
#include "EscapeStalingradZ/widget/WDicesCombat.h"
#include "EscapeStalingradZ/widget/WDiceSpreadCombat.h"
#include "EscapeStalingradZ/widget/WSearchDie.h"
#include "EscapeStalingradZ/widget/UserHUD.h"
#include "Components/WidgetComponent.h"
#include "EscapeStalingradZ/misc/StunIcon.h"
#include "Kismet/GameplayStatics.h"
#include "EscapeStalingradZ/misc/AnimatedTextAttack.h"
#include "EscapeStalingradZ/turn/Turn.h"

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

	UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr)
	{
		if (isPrimaryPlayer) {
			if (GI->playersInfo.Contains(0)) {
				SetAttributesByPlayerInfoSaved(GI->playersInfo[0]);
				if (GI->playersInfo.Contains(1)) {
					APlayerCharacter* character2 = GetWorld()->SpawnActor<APlayerCharacter>(characterClass, FVector(0,0,0), FRotator(0, -90, 0));
					if (character2 != nullptr) {
						character2->startIndex = GetStartIndexSecondCharacter();
					}
				}
			}
		}
		else {
			if (GI->playersInfo.Contains(1)) {
				SetAttributesByPlayerInfoSaved(GI->playersInfo[1]);
			}
		}
	}
	AUserHUD* hud = Cast<AUserHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (GetWorld()->GetName() == "FUBAR") {
		if (isPrimaryPlayer) {
			hud->CreateSelectCharacterWidget(this);
		}
	}
	else {
		if (isPrimaryPlayer) {
			hud->SetPrimaryCharacterToBetweenScenarios(this);
		}
		else {
			if (GetWorld()->GetName() != "AFriendWillBleed") {
				GetWorldTimerManager().SetTimer(initialTimer, this, &APlayerCharacter::SetSecondCharacterBetweenScenarios, 0.2, false);
			}
		}
	}
	SetPreferredWeaponByCharacter();
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
				if (grid->gridTiles[index].wallsHinder.Contains(backward)) {
					res += 2;
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
	if (exp >= 10 && useReadyWeapon && readyWeapon->weaponName == PreferredWeapon) {
		res--;
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
				AAnimatedTextAttack* text = GetWorld()->SpawnActor<AAnimatedTextAttack>(textClass, chara->GetActorLocation(), FRotator(0, 0, 0));
				if (text != nullptr) {
					text->SetAnimationText(NSLOCTEXT("combat", "FriendyFire", "Friendly Fire"));
				}
				chara->health--;
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
	typeOfCovering = CoveringType::NONE;
	zombie->characterInContact = this;
	if (stunIcon == nullptr) {
		FVector pos = (zombie->GetActorLocation() + GetActorLocation()) / 2;
		stunIcon = GetWorld()->SpawnActor<AStunIcon>(stunClass, pos, FRotator(0, 0, 0));
	}
	else if (stunIcon->IsHidden()) {
		FVector pos = (zombie->GetActorLocation() + GetActorLocation()) / 2;
		stunIcon = GetWorld()->SpawnActor<AStunIcon>(stunClass, pos, FRotator(0, 0, 0));
		stunIcon->SetActorHiddenInGame(false);
	}
	FRotator rotacion = GetRotationDirectionToZombie(zombie);
	SetActorRotation(rotacion);
}

void APlayerCharacter::SearchAction()
{
	int die = FMath::RandRange(1, 12);
	CreateOrSetDieSearchWidget(die);
}

void APlayerCharacter::CreateOrSetDieSearchWidget(int numberOfDie)
{
	if (SearchDieWidgetClass) {
		if (SearchDieWidget != nullptr) {
			SearchDieWidget->character = this;
			SearchDieWidget->SetVisibility(ESlateVisibility::Visible);
			SearchDieWidget->SetDie(numberOfDie);
		}
		else {
			SearchDieWidget = CreateWidget<UWSearchDie>(GetWorld(), SearchDieWidgetClass);
			if (SearchDieWidget != nullptr) {
				SearchDieWidget->character = this;
				SearchDieWidget->AddToViewport();
				SearchDieWidget->SetDie(numberOfDie);
			}
		}
	}
}

void APlayerCharacter::SetSecondCharacterBetweenScenarios()
{
	AUserHUD* hud = Cast<AUserHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	hud->SetSecondaryCharacterToBetweenScenarios(this);
}

void APlayerCharacter::SetAttributesByPlayerInfoSaved(FPlayerInfoSaved info)
{
	characterChosen = info.characterChosen;
	health = info.health;
	ammo = info.ammo;
	exp = info.exp;
	food = info.food;
	medkit = info.medkit;
	weapon1 = info.weapon1;
	weapon2 = info.weapon2;
	weapon3 = info.weapon3;
	weapon4 = info.weapon4;
	readyWeapon->SetPropiertiesByName(info.readyWeapon);
	readySecondaryWeapon->SetPropiertiesByName(info.secondaryWeapon);
	useReadyWeapon = info.useReadyWeapon;
}

void APlayerCharacter::SaveAttributesInPlayerInfoSavedGivenIndex(int index)
{
	UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr)
	{
		if (GI->playersInfo.Contains(index)) {
			GI->playersInfo[index] = FPlayerInfoSaved(characterChosen, health, exp, ammo, food, medkit, useReadyWeapon);
		}
		else {
			GI->playersInfo.Add(index, FPlayerInfoSaved(characterChosen, health, exp, ammo, food, medkit, useReadyWeapon));
		}
		GI->playersInfo[index].SetWeapons(weapon1, weapon2, weapon3, weapon4, readyWeapon->weaponName, readySecondaryWeapon->weaponName);
	}
}

FIntPoint APlayerCharacter::GetStartIndexSecondCharacter()
{
	if (GetWorld()->GetName() == "Stash") {
		return FIntPoint(3,9);
	}
	else if (GetWorld()->GetName() == "AFriendWillBleed") {
		return FIntPoint(0,3);
	}
	else {
		return FIntPoint(2,9);
	}
}

void APlayerCharacter::SpawnNewCharacter(AvailableCharacter chosenCharacter)
{
	FIntPoint index = GetStartIndexSecondCharacter();
	FVector pos = grid->GetLocationByIndex(index);
	APlayerCharacter* character2 = GetWorld()->SpawnActor<APlayerCharacter>(characterClass, pos, FRotator(0, 0, 0));
	if (character2 != nullptr) {
		character2->characterChosen = chosenCharacter;
		character2->startIndex = index;
		AUserHUD* hud = Cast<AUserHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		if (hud != nullptr && hud->turn != nullptr) {
			ATurn* turn = hud->turn;
			turn->characters.Add(character2);
			turn->charactersToStartTurn.Add(character2);
		}
	}
}

void APlayerCharacter::ChangePrimaryAndSecondaryWeaponAfterExchange()
{
	TArray<EWeapon> currentsWeapons = TArray<EWeapon>();
	currentsWeapons.Add(weapon1);
	currentsWeapons.Add(weapon2);
	currentsWeapons.Add(weapon3);
	currentsWeapons.Add(weapon4);
	if (!currentsWeapons.Contains(readyWeapon->weaponName)) {
		readyWeapon->SetPropiertiesByName(EWeapon::None);
	}
	if (!currentsWeapons.Contains(readySecondaryWeapon->weaponName)) {
		readySecondaryWeapon->SetPropiertiesByName(EWeapon::None);
		if (!useReadyWeapon) {
			useReadyWeapon = true;
		}
	}
}
