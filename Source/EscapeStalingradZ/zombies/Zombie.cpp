// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "EscapeStalingradZ/grid/Grid.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/turn/Turn.h"
#include "EscapeStalingradZ/widget/WZombieInfo.h"
#include "EscapeStalingradZ/misc/AnimatedTextAttack.h"
#include "EscapeStalingradZ/misc/StunIcon.h"

// Sets default values
AZombie::AZombie()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(FName("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>("InstancedMesh");
	mesh->SetupAttachment(DefaultSceneRoot);

	zombieInfo = CreateDefaultSubobject<UWidgetComponent>("ZombieInfo");
	zombieInfo->SetupAttachment(DefaultSceneRoot);

	SetActorEnableCollision(true);

}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();
	SetHealthAndMPPropertiesByZombie();
	timeMovement *= stepMovementPerSecond;

	UWZombieInfo* zombieWidget = Cast<UWZombieInfo>(zombieInfo->GetWidget());
	if (zombieWidget != nullptr) {
		zombieWidget->zombie = this;
	}
}

void AZombie::SetHealthAndMPPropertiesByZombie()
{
	if (typeOfZombie == ZombieType::Alpha) {
		health = 2;
		mp = 2;
		maxmp = 2;
	}
	else {
		health = 1;
		mp = 1;
		maxmp = 1;
	}
	isStunned = false;
	characterInContact = nullptr;
}

bool AZombie::ZombieHit(int die, int stunNumber)
{
	FVector pos = GetActorLocation();
	AAnimatedTextAttack* text = GetWorld()->SpawnActor<AAnimatedTextAttack>(textClass, pos, FRotator(0, 0, 0));
	if (die >= stunNumber) {
		if (die >= stunNumber + 3) {
			if (text != nullptr) {
				text->SetAnimationText(FText::FromString("Hit"));
			}
			health--;
			if (health <= 0) {
				ZombieKill(pos);
				return true;
			}
			else {
				isStunned = true;
				CheckIfAreCharactersInNeighborWhenStunOrKillZombie(GetActorLocation());
				if (typeOfZombie == ZombieType::Alpha) {
					maxmp = 1;
					mp = 1;
					turn->SetBetaMPWhenAlphaOnBoardGetsHit();
				}
			}
		}
		else {
			if (typeOfZombie == ZombieType::Armoured) {
				isStunned = false;
				if (text != nullptr) {
					text->SetAnimationText(FText::FromString("Miss"));
				}
			}
			else if (typeOfZombie == ZombieType::Kugelfisch) {
				ZombieKill(pos);
				if (text != nullptr) {
					text->SetAnimationText(FText::FromString("Pop")); //meter sonido
				}
				FIntPoint index = grid->GetTileIndexFromLocation(pos);
				grid->SetPoisonedTilesGivenCenterTile(index);
				return true;
			} 
			else {
				if (text != nullptr) {
					text->SetAnimationText(FText::FromString("Stun"));
				}
				isStunned = true;
				CheckIfAreCharactersInNeighborWhenStunOrKillZombie(GetActorLocation());
			}
		}
	}
	else {
		if (text != nullptr) {
			text->SetAnimationText(FText::FromString("Miss"));
		}
	}
	return false;
}

void AZombie::ZombieActions()
{
	if (isStunned) {
		turn->nextZombie();
	}
	else {
		if (characterInContact != nullptr) {
			CoveringAttackBeforeLock();
		}
		else {
			if (!ZombieDirectContact()) {
				for (APlayerCharacter* chara : characters) {
					if (chara->typeOfCovering != CoveringType::NONE) {
						charactersInCovering.Add(chara);
					}
				}
				MovementZombie();
			}
			else {
				CoveringAttackBeforeLock();
			}
		}
	}
}

void AZombie::MovementZombie()
{
	FIntPoint tile = grid->GetTileIndexFromLocation(GetActorLocation());
	if (characters.Num() > 1) {
		int min = 100;
		int indice = 0;
		int cost = 100;
		for (int i = 0; i < characters.Num(); i++) {
			cost = GetMinimunCostBetweenTwoTiles(tile,
				grid->GetTileIndexFromLocation(characters[i]->GetActorLocation()));
			if (cost < min) {
				min = cost;
				indice = i;
			}
		}
		pathToCharacter = FindPath(tile,
			grid->GetTileIndexFromLocation(characters[indice]->GetActorLocation()));
	}
	else {
		if (characters.Num() > 0) {
			pathToCharacter = FindPath(tile,
				grid->GetTileIndexFromLocation(characters[0]->GetActorLocation()));
		}
	}
	if (pathToCharacter.Num() > 0) {
		MoveZombieToNextLocation(0);
	}
	else {
		turn->nextZombie();
	}
}

bool AZombie::MoveZombie(FIntPoint targetTile)
{
	if (grid->gridTiles[targetTile].actor != nullptr 
		|| grid->gridTiles[targetTile].types.Contains(TileType::Hinder)) {
		return false;
	}
	FIntPoint currentTile = grid->GetTileIndexFromLocation(GetActorLocation());
	grid->gridTiles[currentTile].actor = nullptr;
	grid->gridTiles[targetTile].actor = this;
	targetLocation = grid->GetLocationByIndex(targetTile);
	currentLocation = GetActorLocation();
	return true;
}

void AZombie::MoveZombieLocation()
{
	cont++;
	currentMovementTime = cont / timeMovement;
	SetActorLocation(FMath::Lerp(currentLocation, targetLocation, currentMovementTime));
	if (cont == timeMovement) {
		GetWorldTimerManager().ClearTimer(movementTimer);
		cont = 0;
		currentMovementTime = 0;
		currentIndexPath++;
		mp--;
		FIntPoint previousIndex = grid->GetTileIndexFromLocation(currentLocation);
		turn->SpawnWaitingZombies(previousIndex);
		CoveringAttack();
	}
}

void AZombie::MoveZombieToNextLocation(int indexInPath)
{
	if (GetActorLocation().Z > -1000 && !isStunned) {
		if (ZombieDirectContact()) {
			currentIndexPath = 0;
			characterInContact->ZombieLock(this);
			mp = maxmp;
			turn->nextZombie();
		}
		else {
			if (mp > 0 && MoveZombie(pathToCharacter[indexInPath])) {
				GetWorldTimerManager().SetTimer(movementTimer, this, &AZombie::MoveZombieLocation, 1. / stepMovementPerSecond, true);
			}
			else {
				mp = maxmp;
				currentIndexPath = 0;
				turn->nextZombie();
			}
		}
	}
	else {
		mp = maxmp;
		currentIndexPath = 0;
		turn->nextZombie();
	}
}

void AZombie::CoveringAttack()
{
	int nCharacterInCovering = charactersInCovering.Num();
	if (charactersInCovering.Num() > 0) {
		APlayerCharacter* chara = charactersInCovering[0];
		FIntPoint start = grid->GetTileIndexFromLocation(GetActorLocation());
		if (chara->typeOfCoveringAttack == CoveringAttackType::HandToHand) {
			FIntPoint end = grid->GetTileIndexFromLocation(chara->GetActorLocation());
			TArray<FIntPoint> indices;
			if (chara->GetDistanceAttackHandToHand() == 1) {
				indices = grid->GetTileNeighbors(end);
			}
			else {
				indices = chara->GetIndexHandToHand2Range();
			}
			bool inRange = false;
			for (FIntPoint index : indices) {
				if (grid->gridTiles[index].actor == this) {
					inRange = true;
					break;
				}
			}
			if(inRange) {
				chara->CoveringAttackHandToHand(this, start);
			}
			else {
				charactersInCovering.Remove(chara);
			}
		}
		else {
			chara->getArcOfFire();
			if (chara->LoFs.Contains(grid->GetTileIndexFromLocation(GetActorLocation()))) {
				if(chara->typeOfCoveringAttack == CoveringAttackType::NormalFire) {
					chara->CoveringAttackNormalFire(this, start);
				}
				else {
					chara->CoveringAttackSpreadFire(this, start);
				}
			}
			else {
				charactersInCovering.Remove(chara);
			}
		}
	}
	if (charactersInCovering.Num() <= 0) {
		MoveZombieToNextLocation(currentIndexPath);
	}
	else {
		if (nCharacterInCovering != charactersInCovering.Num()) { //el personaje no puede atacar en covering, pasa al siguiente
			CoveringAttack();
		}
	}
}

bool AZombie::ZombieDirectContact()
{
	FIntPoint tile = grid->GetTileIndexFromLocation(GetActorLocation());
	characterInContact = grid->CharacterInNeighbor(tile);
	if (characterInContact != nullptr) {
		return true;
	}
	return false;
}

void AZombie::CoveringAttackBeforeLock()
{
	if (characterInContact->isLocked || characterInContact->attacked) {
		characterInContact->ZombieLock(this);
		turn->nextZombie();
	}
	else {
		FIntPoint tile = grid->GetTileIndexFromLocation(GetActorLocation());
		if (characterInContact->typeOfCoveringAttack == CoveringAttackType::HandToHand) {
			characterInContact->CoveringAttackHandToHand(this, tile);
		}
		else {
			characterInContact->getArcOfFire();
			if (characterInContact->LoFs.Contains(grid->GetTileIndexFromLocation(GetActorLocation()))) {
				if (characterInContact->typeOfCoveringAttack == CoveringAttackType::NormalFire) {
					characterInContact->CoveringAttackNormalFire(this, tile);
				}
				else {
					characterInContact->CoveringAttackSpreadFire(this, tile);
				}
			}
			else {
				characterInContact->ZombieLock(this);
				turn->nextZombie();
			}
		}
	}
}

void AZombie::CheckIfAreCharactersInNeighborWhenStunOrKillZombie(FVector location)
{
	FIntPoint index = grid->GetTileIndexFromLocation(location);
	for (FIntPoint i : grid->GetTileNeighbors(index)) {
		if (grid->gridTiles[i].actor != nullptr) {
			APlayerCharacter* chara = Cast<APlayerCharacter>(grid->gridTiles[i].actor);
			if (chara != nullptr) {
				FreeCharacterWhenStunOrKillZombie(chara);
			}
		}
	}
}

void AZombie::FreeCharacterWhenStunOrKillZombie(APlayerCharacter* chara)
{
	if (grid != nullptr) {
		chara->isLocked = false;
		chara->inDirectContact = false;
		if (chara->stunIcon != nullptr) {
			chara->stunIcon->SetActorHiddenInGame(true);
		}
		TArray<FIntPoint> neighbors = grid->GetTileNeighbors(grid->GetTileIndexFromLocation(chara->GetActorLocation()));
		for (FIntPoint n : neighbors) {
			AZombie* z = Cast<AZombie>(grid->gridTiles[n].actor);
			if (z != nullptr && !z->isStunned) {
				chara->inDirectContact = true;
				break;
			}
		}
		characterInContact = nullptr;
	}
}

void AZombie::ZombieKill(FVector pos)
{
	FIntPoint index = grid->GetTileIndexFromLocation(pos);
	turn->zombiesDied.Add(this);
	turn->zombies.Remove(this);
	SetActorLocation(FVector(-9999, -9999, -9999));
	grid->gridTiles[index].actor = nullptr;
	SetHealthAndMPPropertiesByZombie();
	CheckIfAreCharactersInNeighborWhenStunOrKillZombie(pos);
	if (typeOfZombie == ZombieType::Alpha) {
		AActor* oIcon = GetWorld()->SpawnActor<AActor>(searchOClass, pos, FRotator(0, 0, 0));
		grid->gridTiles[index].types.Add(TileType::Search);
	}
}

TArray<FIntPoint> AZombie::FindPath(FIntPoint start, FIntPoint end)
{
	ClearPathFindingData();
	target = end;
	if (end != FIntPoint(-1, -1) && start != end) {
		DiscoverTile(FPathFinding(start,1,0, GetMinimunCostBetweenTwoTiles(start,end), FIntPoint(-1,-1)));
		while (discoveredTiles.Num() > 0) {
			if (AnalyseNextDiscoverTile()) {
				return GeneratePath(start, end);
			}
		}
		return TArray<FIntPoint>();
	}
	else {
		return TArray<FIntPoint>();
	}
	
}

void AZombie::DiscoverTile(FPathFinding pathFinding)
{
	pathFindingData.Add(pathFinding.index, pathFinding);
	InsertTileInDiscoveredTiles(pathFinding);
}

int AZombie::GetMinimunCostBetweenTwoTiles(FIntPoint start, FIntPoint end)
{
	return abs(start.X - end.X) + abs(start.Y - end.Y);
}

bool AZombie::AnalyseNextDiscoverTile()
{
	currentDiscoveredTile = PullOutCheapestTileOfDiscoveredList();
	currentNeighbors = GetValidNeighbors(currentDiscoveredTile.index);
	while (currentNeighbors.Num() > 0) {
		if (NeighborIsTarget()) {
			return true;
		}
	}
	return false;
}

TArray<FIntPoint> AZombie::GeneratePath(FIntPoint start, FIntPoint end)
{
	TArray<FIntPoint> path;
	TArray<FIntPoint> inversedPath;
	while (start != end) {
		inversedPath.Add(end);
		end = pathFindingData[end].previousIndex;
	}
	for (int i = inversedPath.Num() - 1; i >= 0; i--) {
		path.Add(inversedPath[i]);
	}
	return path;
}
FPathFinding AZombie::PullOutCheapestTileOfDiscoveredList()
{
	FIntPoint index = discoveredTiles[0];
	discoveredTiles.RemoveAt(0);
	discoveredTilesSortingCosts.RemoveAt(0);
	analysedTiles.Add(index);
	return pathFindingData[index];
}

TArray<FPathFinding> AZombie::GetValidNeighbors(FIntPoint index)
{
	TArray<FIntPoint> neighborsTiles = grid->GetTileNeighbors(index);
	TArray<FPathFinding> validNeighbors;
	for (FIntPoint neighbor : neighborsTiles) {
		int cost = grid->GetCostToEnterNeighbor(index, neighbor);
		if (cost > 0) {
			validNeighbors.Add(FPathFinding(neighbor, cost, 99999, 99999, index));
		}
	}
	return validNeighbors;
}

bool AZombie::NeighborIsTarget()
{
	currentNeighbor = currentNeighbors[0];
	currentNeighbors.RemoveAt(0);
	if (!analysedTiles.Contains(currentNeighbor.index)) {
		int cost = currentDiscoveredTile.costFromStart + currentNeighbor.costToEnterTile;
		int indexDiscovered = discoveredTiles.Find(currentNeighbor.index);
		if (indexDiscovered >= 0) {
			currentNeighbor = pathFindingData[currentNeighbor.index]; 
			if (cost >= currentNeighbor.costFromStart) {
				return false;
			}
			discoveredTiles.RemoveAt(indexDiscovered);
			discoveredTilesSortingCosts.RemoveAt(indexDiscovered);
		}
		FPathFinding pathF = FPathFinding(currentNeighbor.index, currentNeighbor.costToEnterTile,
			cost, GetMinimunCostBetweenTwoTiles(currentNeighbor.index, target), currentDiscoveredTile.index);
		DiscoverTile(pathF);
		if (currentNeighbor.index == target) {
			return true;
		}
	}
	return false;
}

void AZombie::InsertTileInDiscoveredTiles(FPathFinding pathFinding)
{
	FIntPoint index = pathFinding.index;
	int sortingCost = pathFinding.costFromStart + pathFinding.minimumCostToTarget;
	if (discoveredTilesSortingCosts.Num() == 0) {
		discoveredTilesSortingCosts.Add(sortingCost);
		discoveredTiles.Add(index);
	}
	else {
		if (sortingCost >= discoveredTilesSortingCosts[discoveredTilesSortingCosts.Num() - 1]) {
			discoveredTilesSortingCosts.Add(sortingCost);
			discoveredTiles.Add(index);
		}
		else {
			for (int i = 0; i < discoveredTilesSortingCosts.Num(); i++) {
				if (sortingCost <= discoveredTilesSortingCosts[i]) {
					discoveredTilesSortingCosts.Insert(sortingCost, i);
					discoveredTiles.Insert(index, i);
					break;
				}
			}
		}
	}
}

void AZombie::ClearPathFindingData()
{
	discoveredTiles.Empty();
	discoveredTilesSortingCosts.Empty();
	analysedTiles.Empty();
	currentNeighbors.Empty();
	pathFindingData.Empty();
}

