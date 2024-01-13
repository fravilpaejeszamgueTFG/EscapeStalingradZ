// Fill out your copyright notice in the Description page of Project Settings.


#include "GridModifier.h"
#include "Grid.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AGridModifier::AGridModifier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(FName("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>("InstancedMesh");
	mesh->SetupAttachment(DefaultSceneRoot);

	boxCollision = CreateDefaultSubobject<UBoxComponent>("boxCollision");
	boxCollision->SetupAttachment(mesh);

	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);

}

// Called when the game starts or when spawned
void AGridModifier::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(modifierTimer, this, &AGridModifier::ActivateCollision, 1, false);
	OnActorBeginOverlap.AddDynamic(this, &AGridModifier::OnOverlap);
}

void AGridModifier::ActivateCollision()
{
	SetActorEnableCollision(true);
}

void AGridModifier::OnOverlap(AActor* me, AActor* other)
{
	AGrid* grid = Cast<AGrid>(other);
	if (grid != nullptr) {
		FIntPoint index = grid->GetTileIndexFromLocation(GetActorLocation());
		grid->gridTiles[index].types.Remove(TileType::Normal);
		grid->gridTiles[index].types.Add(type);
		Destroy();
	}
}

