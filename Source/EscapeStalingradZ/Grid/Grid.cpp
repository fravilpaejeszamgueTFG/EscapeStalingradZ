// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EscapeStalingradZ/utilities/UtilitiesESZ.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/zombies/Zombie.h"
#include "Particles/ParticleSystemComponent.h"
#include "EscapeStalingradZ/player/PlayerC.h"
#include "EscapeStalingradZ/player/PlayerActions.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(FName("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	instancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("InstancedMesh");
	instancedMesh->SetupAttachment(DefaultSceneRoot);

	particleLoF = CreateDefaultSubobject<UParticleSystemComponent>("particleLoF");
	particleLoF->SetupAttachment(DefaultSceneRoot);
	particleLoF->bAutoActivate = false;

	instancedMesh->NumCustomDataFloats = 4;

}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();

	SpawnGrid(GetActorLocation(), tileScale, numberOfTiles);

	APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (player != nullptr && player->actions!=nullptr) {
		player->actions->grid = this;
	}
	
}

void AGrid::SpawnGrid(FVector center, FVector tileSize, FVector2D numOfTiles)
{
	if (gridTiles.Num() > 0) {
		gridTiles.Reset();
	}
	instancedMesh->ClearInstances();
	centerOfGrid = center;
	tileScale = tileSize;
	numberOfTiles = FVector2D(round(numOfTiles.X), round(numOfTiles.Y));
	gridBottomLeftCornerLocation = center - (tileSize * FVector(numOfTiles,0.) / 2);
	for (int i = 0; i < numberOfTiles.X; i++) {
		for (int j = 0; j < numberOfTiles.Y; j++) {
			FTransform transform = FTransform(FRotator(0, 0, 0),
				gridBottomLeftCornerLocation + tileSize * FVector(i, j, 0) + FVector(0,0,0), tileScale / meshSize);
			instancedMesh->AddInstanceWorldSpace(transform);
			FIntPoint index = FIntPoint(i, j);
			ChangeTileData(index, FTileData(index,TileType::Normal,false));
		}
	}
}

FVector AGrid::GetCursorLocationOnGrid() 
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FHitResult hit;
	if (controller->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, hit)) {
		return hit.Location;
	}
	else {
		return FVector(-1000, -1000, -1000);
	}
}

TArray<FIntPoint> AGrid::GetTileNeighbors(FIntPoint index)
{
	TArray<FIntPoint> list = TArray<FIntPoint>();
	if (index.X > 0) {
		list.Add(index - FIntPoint(1, 0));
	}
	if (index.Y > 0) {
		list.Add(index - FIntPoint(0, 1));
	}
	if (index.X < numberOfTiles.X - 1) {
		list.Add(index + FIntPoint(1, 0));
	}
	if (index.Y < numberOfTiles.Y - 1) {
		list.Add(index + FIntPoint(0, 1));
	}
	return list;
}

TArray<FIntPoint> AGrid::GetFrontTiles(FIntPoint index, FVector forwardVector, FVector rightVector)
{
	TArray<FIntPoint> list = TArray<FIntPoint>();
	FIntPoint forward = index + FIntPoint(round(forwardVector.X), round(forwardVector.Y));
	FIntPoint right = index + FIntPoint(round(rightVector.X), round(rightVector.Y));
	FIntPoint left = index - FIntPoint(round(rightVector.X), round(rightVector.Y));
	if (forward.X >= 0 && forward.X < numberOfTiles.X && forward.Y >= 0 && forward.Y < numberOfTiles.Y) {
		list.Add(forward);
	}
	if (right.X >= 0 && right.X < numberOfTiles.X && right.Y >= 0 && right.Y < numberOfTiles.Y) {
		list.Add(right);
	}
	if (left.X >= 0 && left.X < numberOfTiles.X && left.Y >= 0 && left.Y < numberOfTiles.Y) {
		list.Add(left);
	}
	return list;
}

FIntPoint AGrid::GetTileIndexFromLocation(FVector location)
{
	FVector v = UtilitiesESZ::SnapVectorToVector(location - gridBottomLeftCornerLocation, tileScale)/tileScale;
	return FIntPoint(v.X, v.Y);
}

void AGrid::UpdateTileNeighbors(FIntPoint index, bool isadding)
{
	TArray<FIntPoint> neighbors = GetTileNeighbors(index);
	for (FIntPoint neighbor : neighbors) {
		if (isadding) {
			AddTileState(neighbor, TileState::isNeighbor);
		}
		else {
			RemoveTileState(neighbor, TileState::isNeighbor);
		}
	}
}

FIntPoint AGrid::GetTileIndexUnderCursor()
{
	return GetTileIndexFromLocation(GetCursorLocationOnGrid());
}

void AGrid::AddTileState(FIntPoint index, TileState state)
{
	if (gridTiles.Contains(index)) {
		if (!gridTiles[index].states.Contains(state)) {
			gridTiles[index].states.Add(state);
			UpdateTileVisual(index);
		}
	}
}

void AGrid::RemoveTileState(FIntPoint index, TileState state)
{
	if (gridTiles.Contains(index)) {
		if (gridTiles[index].states.Contains(state)) {
			gridTiles[index].states.Remove(state);
			UpdateTileVisual(index);
		}
	}
}

void AGrid::UpdateTileVisual(FIntPoint index)
{
	FLinearColor color = GetColorFromState(gridTiles[index].states);
	int i = index.X * numberOfTiles.Y + index.Y;
	instancedMesh->SetCustomDataValue(i,0,color.R, true);
	instancedMesh->SetCustomDataValue(i,1,color.G, true);
	instancedMesh->SetCustomDataValue(i,2,color.B, true);
	if (color == tileAoFColor) {
		instancedMesh->SetCustomDataValue(i,3,0.05,true);
	}
	else {
		instancedMesh->SetCustomDataValue(i,3,0,true);
	}
}

FLinearColor AGrid::GetColorFromState(TArray<TEnumAsByte<TileState>> states)
{
	if (states.Num() > 0) {
		if (states.Contains(TileState::Selected)) {
			return tileSelectedColor;
		}
		if (states.Contains(TileState::Hovered)) {
			return tileHoveredColor;
		}
		if (states.Contains(TileState::isReachable)) {
			return tileReachableColor;
		}
		if (states.Contains(TileState::isInAoF)) {
			return tileAoFColor;
		}
		if (states.Contains(TileState::isNeighbor)) {
			return tileNeighborColor;
		}
	}
	return tileNoneColor;
}


void AGrid::ChangeTileData(FIntPoint index, FTileData data)
{
	if (gridTiles.Contains(index)) {
		gridTiles[index] = data;
	}
	else {
		gridTiles.Add(index, data);
	}
}

void AGrid::SetPlayerStartLocation(APlayerCharacter* character)
{
	FIntPoint index = character->startIndex;
	FVector location = GetLocationByIndex(index);
	character->SetActorLocation(location);
	gridTiles[index].actor = character;
	charactersIndex.Add(index);
}

void AGrid::SetZombieStartLocation(AZombie* zombie)
{
	FIntPoint index = zombie->startIndex;
	FVector location = GetLocationByIndex(index);
	zombie->SetActorLocation(location);
	gridTiles[index].actor = zombie;
}

FIntPoint AGrid::GetStartIndex()
{
	//TO-DO
	return FIntPoint(7, 6);
}

FVector AGrid::GetLocationByIndex(FIntPoint index)
{
	return FVector(gridBottomLeftCornerLocation + tileScale * FVector(index.X, index.Y, 0));
}

TArray<FIntPoint> AGrid::GetTilesForward(FIntPoint index, FVector forwardVector, int numCasillas)
{
	TArray<FIntPoint> list = TArray<FIntPoint>();
	for (int i = 1; i <= numCasillas; i++) {
		FIntPoint forward = index + FIntPoint(round(forwardVector.X) * i, round(forwardVector.Y) * i);
		if (forward.X >= 0 && forward.X < numberOfTiles.X && forward.Y >= 0 && forward.Y < numberOfTiles.Y) {
			list.Add(forward);
		}
		else {
			break;
		}
	}
	return list;
}

TArray<FIntPoint> AGrid::GetTilesForwardMovement(FIntPoint index, FVector forwardVector, int numCasillas, int mp, int cost)
{
	TArray<FIntPoint> list = TArray<FIntPoint>();
	int cont = 0;
	for (int i = 1; i <= numCasillas; i++) {
		FIntPoint forward = index + FIntPoint(round(forwardVector.X) * i, round(forwardVector.Y) * i);
		if (forward.X >= 0 && forward.X < numberOfTiles.X && forward.Y >= 0 && forward.Y < numberOfTiles.Y) {
			if (gridTiles[forward].actor != nullptr || gridTiles[forward].types.Contains(TileType::Fire)) {
				break;
			}
			if (gridTiles[forward].walls.Num() > 0) {
				FIntPoint backward = index + FIntPoint(round(forwardVector.X) * (i - 1), round(forwardVector.Y) * (i - 1));
				if (gridTiles[forward].walls.Contains(backward)) {
					break;
				}
			}
			FIntPoint backward = index + FIntPoint(round(forwardVector.X) * (i - 1), round(forwardVector.Y) * (i - 1));
			if (GetDoorIsClosed(forward, backward)) {
				break;
			}
			if (gridTiles[forward].types.Contains(TileType::Hinder)) {
				cont++;
			}
			if (i * cost + cont <= mp) {
				list.Add(forward);
			}
		}
		else {
			break;
		}
	}
	return list;
}

void AGrid::SetTilesForAttack(TArray<FIntPoint> list)
{
	for (FIntPoint j : list) {
		AddTileState(j, TileState::isInAoF);
	}
}

TArray<FIntPoint> AGrid::GetTilesAoF(FIntPoint index, FVector forwardVector, FVector rightVector)
{
	TArray<FIntPoint> list = TArray<FIntPoint>();
	int distanceForAoF = GetDistanceAoF(index, forwardVector);
	for (int i = 1; i <= distanceForAoF; i++) {
		FIntPoint forward = FIntPoint(round(forwardVector.X) * i, round(forwardVector.Y) * i);
		TArray<FIntPoint> adjacentForward = GetAdjacentForward(index + forward, i, rightVector);
		list.Add(index + forward);
		list.Append(adjacentForward);
	}
	return list;
}

int AGrid::GetDistanceAoF(FIntPoint index, FVector forwardVector)
{
	if (round(forwardVector.X) == 1) {
		return numberOfTiles.X - index.X - 1;
	}
	else if (round(forwardVector.X) == -1) {
		return index.X;
	}
	else if (round(forwardVector.Y) == 1) {
		return numberOfTiles.Y - index.Y - 1;
	}
	else {
		return index.Y;
	}
}

TArray<FIntPoint> AGrid::GetAdjacentForward(FIntPoint index, int iterator, FVector rightVector)
{
	TArray<FIntPoint> list = TArray<FIntPoint>();
	TArray<FIntPoint> right = GetTilesForward(index, rightVector, iterator);
	TArray<FIntPoint> left = GetTilesForward(index, -rightVector, iterator);
	list.Append(right);
	list.Append(left);
	return list;
}

TArray<FIntPoint> AGrid::GetTilesLoF(FIntPoint start, FIntPoint end)
{
	TArray<FIntPoint> list = TArray<FIntPoint>();
	if (!gridTiles[end].states.Contains(TileState::isInAoF)) {
		return list;
	}
	FVector Start = GetLocationByIndex(start);
	FVector End = GetLocationByIndex(end);
	FVector vector = End - Start;
	FVector vectorNormal = vector.GetSafeNormal();
	FIntPoint lastIndex = FIntPoint(-1, -1);
	for (int i = 1; i < vector.Size() - 10; i++) {
		FIntPoint index = GetTileIndexFromLocation(Start + (i * vectorNormal));
		if (!list.Contains(index)) {
			if (lastIndex != FIntPoint(-1, -1) && lastIndex.X !=index.X && lastIndex.Y != index.Y) {
				if (!CanShootDiagonal(lastIndex, FIntPoint(lastIndex.X, index.Y), FIntPoint(index.X, lastIndex.Y), index)) {
					return TArray<FIntPoint>();
				}
			}
			else {
				if (gridTiles[index].walls.Num() > 0) {
					for (FIntPoint j : list) {
						if (gridTiles[index].walls.Contains(j)) {
							return TArray<FIntPoint>();
						}
					}
				}
				if (gridTiles[index].doors.Num() > 0) {
					for (FIntPoint j : list) {
						if (gridTiles[index].doors.Contains(j)) {
							if (gridTiles[index].doors[j]) {
								return TArray<FIntPoint>();
							}
						}
					}
				}
			}
			list.Add(index);
			lastIndex = index;
		}
	}
	list.Remove(start);
	return list;
}

TArray<FIntPoint> AGrid::GetTilesDiagonals(FIntPoint index, FVector forwardVector, FVector rightVector, int numCasillas, int mp)
{
	TArray<FIntPoint> list = TArray<FIntPoint>();
	int cont = 0;
	for (int i = 1; i <= numCasillas; i++) {
		FIntPoint forward = FIntPoint(round(forwardVector.X) * i, round(forwardVector.Y) * i);
		FIntPoint right = FIntPoint(round(rightVector.X) * i, round(rightVector.Y) * i);
		FIntPoint tile = index + forward + right;
		if (tile.X >= 0 && tile.X < numberOfTiles.X && tile.Y >= 0 && tile.Y < numberOfTiles.Y) {
			if (gridTiles[tile].actor != nullptr || gridTiles[tile].types.Contains(TileType::Fire)) {
				break;
			}
			FIntPoint fw = FIntPoint(round(rightVector.X), round(rightVector.Y));
			FIntPoint rw = FIntPoint(round(forwardVector.X), round(forwardVector.Y));
			forward = tile - fw;
			FIntPoint backward = tile - fw - rw;
			right = tile - rw;
			if (!CanMoveDiagonal(tile, forward, right, backward)) {
				break;
			}
			if (gridTiles[forward].types.Contains(TileType::Fire) || gridTiles[right].types.Contains(TileType::Fire)) {
				break;
			}
			if (gridTiles[tile].types.Contains(TileType::Hinder)) {
				cont++;
			}
			if (i * 2 + cont <= mp) {
				list.Add(tile);
			}
		}
		else {
			break;
		}
	}
	return list;
}

TArray<FIntPoint> AGrid::GetTilesRotation(FIntPoint index, FVector forwardVector, FVector rightVector, int mp)
{
	TArray<FIntPoint> list = TArray<FIntPoint>();
	if (mp >= 2) {
		FIntPoint behind = index + FIntPoint(round(-forwardVector.X), round(-forwardVector.Y));
		if (behind.X >= 0 && behind.X < numberOfTiles.X && behind.Y >= 0 && behind.Y < numberOfTiles.Y) {
			list.Add(behind);
		}
	}
	if (mp >= 1) {
		FIntPoint left = index + FIntPoint(round(-rightVector.X), round(-rightVector.Y));
		if (left.X >= 0 && left.X < numberOfTiles.X && left.Y >= 0 && left.Y < numberOfTiles.Y) {
			list.Add(left);
		}
		FIntPoint right = index + FIntPoint(round(rightVector.X), round(rightVector.Y));
		if (right.X >= 0 && right.X < numberOfTiles.X && right.Y >= 0 && right.Y < numberOfTiles.Y) {
			list.Add(right);
		}
	}
	return list;
}

void AGrid::SetParticleLoF(FIntPoint start, FIntPoint end)
{
	if (!particleLoF->IsActive()) {
		particleLoF->Activate();
	}
	particleLoF->SetVisibility(true);
	particleLoF->SetBeamSourcePoint(0, GetLocationByIndex(start) + FVector(0, 0, 1), 0);
	particleLoF->SetBeamTargetPoint(0, GetLocationByIndex(end) + FVector(0, 0, 1), 0);
}

void AGrid::DeActivateParticleLoF()
{
	//particleLoF->Deactivate();
	particleLoF->SetVisibility(false);
}

TArray<FIntPoint> AGrid::GetTilesWithZombies(TArray<FIntPoint> AoF)
{
	TArray<FIntPoint> list = TArray<FIntPoint>();
	for (FIntPoint index : AoF) {
		if (gridTiles[index].actor != nullptr) {
			AZombie* zombie = Cast<AZombie>(gridTiles[index].actor);
			if (zombie != nullptr) {
				list.Add(index);
			}
		}
	}
	return list;
}

bool AGrid::GetDoorIsClosed(FIntPoint index, FIntPoint other)
{
	if (gridTiles[index].doors.Num() > 0) {
		if (gridTiles[index].doors.Contains(other)) {
			if (gridTiles[index].doors[other]) {
				return true;
			}
		}
	}
	return false;
}

bool AGrid::CanMoveDiagonal(FIntPoint tile, FIntPoint forward, FIntPoint right, FIntPoint backward)
{
	if (gridTiles[forward].walls.Num() > 0) {
		if (gridTiles[forward].walls.Contains(backward) || gridTiles[forward].walls.Contains(tile)) {
			return false;
		}
	}
	if (gridTiles[right].walls.Num() > 0) {
		if (gridTiles[right].walls.Contains(backward) || gridTiles[right].walls.Contains(tile)) {
			return false;
		}
	}
	if (GetDoorIsClosed(forward, backward) || GetDoorIsClosed(forward, tile)
		|| GetDoorIsClosed(right, backward) || GetDoorIsClosed(right, tile)) {
		return false;
	}
	return true;
}

void AGrid::deleteStatesFromTiles()
{
	for (auto& index : gridTiles)
	{
		if (index.Value.states.Num() > 0) {
			index.Value.states.Empty();
			UpdateTileVisual(index.Key);
		}
	}
}
void AGrid::deleteStatesFromTilesButSelected()
{
	for (auto& index : gridTiles)
	{
		if (index.Value.states.Num() > 0) {
			if (!index.Value.states.Contains(TileState::Selected)) {
				index.Value.states.Empty();
			}
			else {
				index.Value.states.Empty();
				index.Value.states.Add(TileState::Selected);
			}
			UpdateTileVisual(index.Key);
		}
	}
}

bool AGrid::CanShootDiagonal(FIntPoint tile, FIntPoint forward, FIntPoint right, FIntPoint backward)
{
	int res = 0;
	bool f = false;
	bool r = false;
	//comprobar que hay pared/puerta en las casillas adyacentes, pero no con la misma
	if (gridTiles[forward].walls.Num() > 0) {
		if (gridTiles[forward].walls.Contains(backward)) {
			f = true;
		}
	}
	if (GetDoorIsClosed(forward, backward)) {
		f = true;
	}
	//forward-backward
	if (gridTiles[right].walls.Num() > 0) {
		if (gridTiles[right].walls.Contains(tile)) {
			r = true;
		}
	}
	if (GetDoorIsClosed(right, tile)) {
		r = true;
	}
	//right-tile
	if (f && r) {
		return false;
	}
	f = false;
	r = false;
	if (gridTiles[forward].walls.Num() > 0) {
		if (gridTiles[forward].walls.Contains(tile)) {
			f = true;
		}
	}
	if (GetDoorIsClosed(forward, tile)) {
		f = true;
	}
	//forward-tile
	if (gridTiles[right].walls.Num() > 0) {
		if (gridTiles[right].walls.Contains(backward)) {
			r = true;
		}
	}
	if (GetDoorIsClosed(right, backward)) {
		r = true;
	}
	//right-backward
	if (f && r) {
		return false;
	}
	return true;
}
