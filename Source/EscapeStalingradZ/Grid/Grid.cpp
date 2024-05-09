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
#include "EscapeStalingradZ/widget/UserHUD.h"
#include "EscapeStalingradZ/turn/Turn.h"
#include "EscapeStalingradZ/misc/AnimatedTextAttack.h"
#include "EscapeStalingradZ/character/CharacterToFree.h"

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
			ChangeTileData(index, FTileData(index,TileType::Normal));
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
		FIntPoint left = index - FIntPoint(1, 0);
		if (!DoorOrWallBetweenTiles(left, index)) {
			list.Add(left);
		}
	}
	if (index.Y > 0) {
		FIntPoint back = index - FIntPoint(0, 1);
		if (!DoorOrWallBetweenTiles(back, index)) {
			list.Add(back);
		}
	}
	if (index.X < numberOfTiles.X - 1) {
		FIntPoint right = index + FIntPoint(1, 0);
		if (!DoorOrWallBetweenTiles(right, index)) {
			list.Add(right);
		}
	}
	if (index.Y < numberOfTiles.Y - 1) {
		FIntPoint forward = index + FIntPoint(0, 1);
		if (!DoorOrWallBetweenTiles(forward, index)) {
			list.Add(forward);
		}
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
	if (v.X >= 0 && v.X < numberOfTiles.X && v.Y >= 0 && v.Y < numberOfTiles.Y) {
		return FIntPoint(v.X, v.Y);
	}
	else {
		return FIntPoint(-1,-1);
	}
	
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
	instancedMesh->SetCustomDataValue(i, 0, color.R, true);
	instancedMesh->SetCustomDataValue(i, 1, color.G, true);
	instancedMesh->SetCustomDataValue(i, 2, color.B, true);
	if (color != tileNoneColor) {
		instancedMesh->SetCustomDataValue(i, 3, 0.25, true);
	}
	else {
		instancedMesh->SetCustomDataValue(i, 3, 0, true);
	}
}

FLinearColor AGrid::GetColorFromState(TArray<TEnumAsByte<TileState>> states)
{
	if (states.Num() > 0) {
		if (states.Contains(TileState::Hovered)) {
			return tileHoveredColor;
		}
		if (states.Contains(TileState::isReachable)) {
			return tileReachableColor;
		}
		if (states.Contains(TileState::poisoned)) {
			return tilePoisonedColor;
		}
		if (states.Contains(TileState::Selected)) {
			return tileSelectedColor;
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

void AGrid::SetCharacterToFreeStartLocation(ACharacterToFree* character)
{
	FIntPoint index = character->startIndex;
	FVector location = GetLocationByIndex(index);
	character->SetActorLocation(location);
	gridTiles[index].actor = character;
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
			FIntPoint backward = index + FIntPoint(round(forwardVector.X) * (i - 1), round(forwardVector.Y) * (i - 1));
			if (gridTiles[forward].walls.Num() > 0) {
				if (gridTiles[forward].walls.Contains(backward)) {
					break;
				}
			}
			if (gridTiles[forward].wallsHinder.Num() > 0) {
				if (gridTiles[forward].wallsHinder.Contains(backward)) {
					break;
				}
			}
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
	if (gridTiles[forward].wallsHinder.Num() > 0) {
		if (gridTiles[forward].wallsHinder.Contains(backward) || gridTiles[forward].wallsHinder.Contains(tile)) {
			return false;
		}
	}
	if (gridTiles[right].wallsHinder.Num() > 0) {
		if (gridTiles[right].wallsHinder.Contains(backward) || gridTiles[right].wallsHinder.Contains(tile)) {
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
			if (!index.Value.states.Contains(TileState::poisoned)) {
				index.Value.states.Empty();
			}
			else {
				index.Value.states.Empty();
				index.Value.states.Add(TileState::poisoned);
			}
			UpdateTileVisual(index.Key);
		}
	}
}
void AGrid::deleteStatesFromTilesButSelected()
{
	bool isPoisoned = false;
	for (auto& index : gridTiles)
	{
		if (index.Value.states.Num() > 0) {
			if (index.Value.states.Contains(TileState::poisoned)) {
				isPoisoned = true;
			}
			else {
				isPoisoned = false;
			}
			if (!index.Value.states.Contains(TileState::Selected)) {
				index.Value.states.Empty();
			}
			else {
				index.Value.states.Empty();
				index.Value.states.Add(TileState::Selected);
			}
			if (isPoisoned){
				index.Value.states.Add(TileState::poisoned);
			}
			UpdateTileVisual(index.Key);
		}
	}
}

void AGrid::DeleteStatesFromTilesButGiven(TArray<FIntPoint> list)
{
	bool isPoisoned = false;
	for (auto& index : gridTiles)
	{
		if (index.Value.states.Num() > 0) {
			if (index.Value.states.Contains(TileState::poisoned)) {
				isPoisoned = true;
			}
			else {
				isPoisoned = false;
			}
			if (list.Contains(index.Key)) {
				index.Value.states.Empty();
				index.Value.states.Add(TileState::Hovered);
			}
			else {
				if (!index.Value.states.Contains(TileState::Selected)) {
					index.Value.states.Empty();
				}
				else {
					index.Value.states.Empty();
					index.Value.states.Add(TileState::Selected);
				}
			}
			if (isPoisoned) {
				index.Value.states.Add(TileState::poisoned);
			}
			UpdateTileVisual(index.Key);
		}
	}
}

int AGrid::GetCostToEnterNeighbor(FIntPoint index, FIntPoint neighbor)
{
	if (gridTiles[neighbor].types.Contains(TileType::Fire) || GetDoorIsClosed(index, neighbor)) {
		return 0;
	} 
	if (gridTiles[neighbor].walls.Num() > 0) {
		if (gridTiles[neighbor].walls.Contains(index)) {
			return 0;
		}
	}
	if (gridTiles[neighbor].wallsHinder.Num() > 0) {
		if (gridTiles[neighbor].wallsHinder.Contains(index)) {
			return 0;
		}
	}
	if (gridTiles[neighbor].types.Contains(TileType::Hinder)) {
		return 500; 
	}
	else {
		if (gridTiles[neighbor].actor != nullptr) {
			AZombie* zombie = Cast<AZombie>(gridTiles[neighbor].actor);
			if (zombie != nullptr) {
				return 100;
			}
			ACharacterToFree* chara = Cast<ACharacterToFree>(gridTiles[neighbor].actor);
			if (chara != nullptr) {
				return 100;
			}
		}
	}
	return 1;
}

bool AGrid::HasZombieInNeighbor(FIntPoint index)
{
	TArray<FIntPoint> vecinos = GetTileNeighbors(index);
	for (FIntPoint v : vecinos) {
		if (gridTiles[v].actor != nullptr) {
			AZombie* zombie = Cast<AZombie>(gridTiles[v].actor);
			if (zombie != nullptr && !zombie->isStunned && zombie->characterInContact == nullptr) {
				return true;
			}
		}
	}
	return false;
}

AZombie* AGrid::ZombieInNeighbor(FIntPoint index)
{
	TArray<FIntPoint> vecinos = GetTileNeighbors(index);
	for (FIntPoint v : vecinos) {
		if (gridTiles[v].actor != nullptr) {
			AZombie* zombie = Cast<AZombie>(gridTiles[v].actor);
			if (zombie != nullptr && !zombie->isStunned && zombie->characterInContact == nullptr) {
				return zombie;
			}
		}
	}
	return nullptr;
}

APlayerCharacter* AGrid::CharacterInNeighbor(FIntPoint index)
{
	TArray<FIntPoint> vecinos = GetTileNeighbors(index);
	for (FIntPoint v : vecinos) {
		if (gridTiles[v].actor != nullptr) {
			APlayerCharacter* chara = Cast<APlayerCharacter>(gridTiles[v].actor);
			if (chara != nullptr && !chara->isLocked) {
				return chara;
			}
		}
	}
	return nullptr;
}

bool AGrid::CanShootDiagonal(FIntPoint tile, FIntPoint forward, FIntPoint right, FIntPoint backward)
{
	int res = 0;
	//comprobar que hay pared/puerta en las casillas adyacentes, pero no con la misma
	bool f = DoorOrWallBetweenTiles(forward, backward);
	bool r = DoorOrWallBetweenTiles(right, tile);
	if (f && r) {
		return false;
	}
	f = DoorOrWallBetweenTiles(forward, tile); 
	r = DoorOrWallBetweenTiles(right, backward); 
	if (f && r) {
		return false;
	}
	//comprobar que hay pared/puerta en las casillas adyacentes de la casilla origen y destino
	f = DoorOrWallBetweenTiles(forward, tile);
	r = DoorOrWallBetweenTiles(right, tile);
	if (f && r) {
		return false;
	}
	f = DoorOrWallBetweenTiles(forward, backward);
	r = DoorOrWallBetweenTiles(right, backward);
	if (f && r) {
		return false;
	}
	return true;
}

bool AGrid::DoorOrWallBetweenTiles(FIntPoint tile1, FIntPoint tile2)
{
	if (gridTiles[tile1].walls.Num() > 0) {
		if (gridTiles[tile1].walls.Contains(tile2)) {
			return true;
		}
	}
	if (GetDoorIsClosed(tile1, tile2)) {
		return true;
	}
	if (gridTiles[tile1].wallsHinder.Num() > 0) {
		if (gridTiles[tile1].wallsHinder.Contains(tile2)) {
			return true;
		}
	}
	return false;
}

TArray<FIntPoint> AGrid::GetTileNeighborsThatCanMoveInto(FIntPoint index)
{
	TArray<FIntPoint> vecinos = GetTileNeighbors(index);
	TArray<FIntPoint> res = TArray<FIntPoint>();
	for (FIntPoint tile : vecinos) {
		if (gridTiles[tile].actor == nullptr && !gridTiles[tile].types.Contains(TileType::Fire)) {
			res.Add(tile);
		}
	}
	return res;
}

void AGrid::EndIfTileIsExit(FIntPoint index)
{
	if (gridTiles[index].types.Contains(TileType::S1) || gridTiles[index].types.Contains(TileType::S2)) {
		APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (player != nullptr) {
			player->hasPrimaryObjective = true;
			AUserHUD* hud = Cast<AUserHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
			if (hud != nullptr && hud->turn != nullptr) {
				if (GetWorld()->GetName() != "MoveAlong") {
					TArray<APlayerCharacter*> characters = hud->turn->characters;
					bool foundPrimaryPLayer = false;
					for (int i = 0; i < characters.Num(); i++) {
						APlayerCharacter* chara = characters[i];
						if (chara->isPrimaryPlayer) {
							foundPrimaryPLayer = true;
							chara->SaveAttributesInPlayerInfoSavedGivenIndex(0);
						}
						else if (foundPrimaryPLayer) {
							chara->SaveAttributesInPlayerInfoSavedGivenIndex(i);
						}
						else {
							chara->SaveAttributesInPlayerInfoSavedGivenIndex(i + 1);
						}
					}
				}
			}
			if (gridTiles[index].types.Contains(TileType::S1)) {
				player->ChangeLevel(0);
			}
			else {
				player->ChangeLevel(1);
			}
		}
	}
}

void AGrid::SetCurrentSearchTileSearched()
{
	if (currentSearchTile != FIntPoint(-1, -1)) {
		FVector pos = GetLocationByIndex(currentSearchTile);
		AActor* oIcon = GetWorld()->SpawnActor<AActor>(searchOClass, pos, FRotator(0, 0, 0));
		if (oIcon != nullptr) {
			currentSearchTile = FIntPoint(-1, -1);
		}
	}
}

void AGrid::SetAllSearchTileSearched()
{
	for (auto& index : gridTiles)
	{
		if (index.Value.types.Contains(TileType::Search)) {
			FVector pos = GetLocationByIndex(index.Key);
			AActor* oIcon = GetWorld()->SpawnActor<AActor>(searchOClass, pos, FRotator(0, 0, 0));
			if (oIcon != nullptr) {
				currentSearchTile = FIntPoint(-1, -1);
			}
			index.Value.types.Remove(TileType::Search);
		}
	}
}

void AGrid::SetPoisonedTilesGivenCenterTile(FIntPoint index)
{
	TArray<FIntPoint> list = GetTileNeighbors(index);
	list.Add(index);
	FIntPoint left = FIntPoint(-1, -1);
	if (index.X > 0) {
		left = index - FIntPoint(1, 0);
	}
	FIntPoint back = FIntPoint(-1, -1);
	if (index.Y > 0) {
		back = index - FIntPoint(0, 1);
	}
	FIntPoint right = FIntPoint(-1, -1);
	if (index.X < numberOfTiles.X - 1) {
		right = index + FIntPoint(1, 0);
	}
	FIntPoint forward = FIntPoint(-1, -1);
	if (index.Y < numberOfTiles.Y - 1) {
		forward = index + FIntPoint(0, 1);
	}
	list.Append(GetTilesDiagonalsGivenNeighbors(index, left, forward, right, back));
	for (FIntPoint tile : list) {
		AddTileState(tile, TileState::poisoned);
	}
}

TArray<FIntPoint> AGrid::GetTilesDiagonalsGivenNeighbors(FIntPoint index, FIntPoint left, FIntPoint forward, FIntPoint right, FIntPoint back)
{
	TArray<FIntPoint> list = TArray<FIntPoint>();
	if (left != FIntPoint(-1, -1) && back != FIntPoint(-1, -1)) {
		FIntPoint diag = index - FIntPoint(1, 1);
		if (CanMoveDiagonal(index, left, back, diag)) {
			list.Add(diag);
		}
	}
	if (left != FIntPoint(-1, -1) && forward != FIntPoint(-1, -1)) {
		FIntPoint diag = index + FIntPoint(-1, 1);
		if (CanMoveDiagonal(index, left, forward, diag)) {
			list.Add(diag);
		}
	}
	if (right != FIntPoint(-1, -1) && back != FIntPoint(-1, -1)) {
		FIntPoint diag = index + FIntPoint(1, -1);
		if (CanMoveDiagonal(index, right, back, diag)) {
			list.Add(diag);
		}
	}
	if (right != FIntPoint(-1, -1) && forward != FIntPoint(-1, -1)) {
		FIntPoint diag = index + FIntPoint(1, 1);
		if (CanMoveDiagonal(index, right, forward, diag)) {
			list.Add(diag);
		}
	}
	return list;
}

void AGrid::DamageIfCharacterInPoisonTile(APlayerCharacter* character)
{
	if (character != nullptr) {
		FVector pos = character->GetActorLocation();
		FIntPoint tile = GetTileIndexFromLocation(pos);
		if (tile != FIntPoint(-1,-1) && gridTiles[tile].states.Contains(TileState::poisoned)) {
			AAnimatedTextAttack* text = GetWorld()->SpawnActor<AAnimatedTextAttack>(textClass, pos, FRotator(0, 0, 0));
			character->health--;
			if (text != nullptr) {
				FString texto = NSLOCTEXT("combat", "Poisoned", "Poisoned, Health").ToString() + "=" + FString::FromInt(character->health);
				text->SetAnimationText(FText::FromString(texto));
			}
			if (character->health <= 0) {
				character->DeathCharacter();
			}
		}
	}
}
