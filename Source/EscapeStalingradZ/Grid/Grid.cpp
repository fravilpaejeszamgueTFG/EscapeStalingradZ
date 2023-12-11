// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EscapeStalingradZ/utilities/UtilitiesESZ.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(FName("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	instancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("InstancedMesh");
	instancedMesh->SetupAttachment(DefaultSceneRoot);

}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();

	SpawnGrid(GetActorLocation(), tileScale, numberOfTiles);
	
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
			ChangeTileData(index, FTileData(index,TileType::Normal,false,false));
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

FIntPoint AGrid::GetTileIndexFromLocation(FVector location)
{
	FVector v = UtilitiesESZ::SnapVectorToVector(location - gridBottomLeftCornerLocation, tileScale)/tileScale;
	return FIntPoint(v.X, v.Y);
}

FIntPoint AGrid::GetTileIndexUnderCursor()
{
	return GetTileIndexFromLocation(GetCursorLocationOnGrid());
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