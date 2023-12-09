// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrid::SpawnGrid(FVector center, FVector tileSize, FVector2D numOfTiles)
{
	instancedMesh->ClearInstances();
	centerOfGrid = center;
	tileScale = tileSize;
	numberOfTiles = numOfTiles;
	gridBottomLeftCornerLocation = center - (tileSize * FVector(numOfTiles,0.) / 2);
	for (int i = 0; i < numberOfTiles.X; i++) {
		for (int j = 0; j < numberOfTiles.Y; j++) {
			FTransform transform = FTransform(FRotator(0, 0, 0),
				gridBottomLeftCornerLocation + tileSize * FVector(i, j, 0), tileScale / meshSize);
			instancedMesh->AddInstanceWorldSpace(transform);
		}
	}
}

