// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "Components/StaticMeshComponent.h"
#include "EscapeStalingradZ/grid/Grid.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/turn/Turn.h"

// Sets default values
AZombie::AZombie()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(FName("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>("InstancedMesh");
	mesh->SetupAttachment(DefaultSceneRoot);

	SetActorEnableCollision(true);

}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();
	SetHealthAndMPPropertiesByZombie();
	timeMovement *= stepMovementPerSecond;
}

void AZombie::SetHealthAndMPPropertiesByZombie()
{
	if (typeOfZombie == ZombieType::Alpha) {
		health = 2;
		mp = 2;
	}
	else {
		health = 1;
		mp = 1;
	}
}

void AZombie::ZombieActions()
{
	//TO-DO hacer acciones de zombies
	MovementZombie();
}

void AZombie::MovementZombie()
{
	FIntPoint tile = grid->GetTileIndexFromLocation(GetActorLocation());
	if (characters.Num() > 1) {
		int min = 100;
		int indice = 0;
		int cost = 100;
		for (int i = 0; i < characters.Num() - 1; i++) {
			cost = GetMinimunCostBetweenTwoTiles(tile,
				grid->GetTileIndexFromLocation(characters[i]->GetActorLocation()));
			if (cost < min) {
				indice = i;
			}
		}
		pathToCharacter = FindPath(tile,
			grid->GetTileIndexFromLocation(characters[indice]->GetActorLocation()));
	}
	else {
		pathToCharacter = FindPath(tile,
			grid->GetTileIndexFromLocation(characters[0]->GetActorLocation()));
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
		MoveZombieToNextLocation(currentIndexPath);
	}
}

void AZombie::MoveZombieToNextLocation(int indexInPath)
{
	if (mp > 0 && MoveZombie(pathToCharacter[indexInPath])) {
		GetWorldTimerManager().SetTimer(movementTimer, this, &AZombie::MoveZombieLocation, 1. / stepMovementPerSecond, true);
	}
	else {
		mp = maxmp;
		currentIndexPath = 0;
		turn->nextZombie();
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

