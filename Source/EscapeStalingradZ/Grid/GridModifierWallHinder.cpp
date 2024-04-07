// Fill out your copyright notice in the Description page of Project Settings.


#include "GridModifierWallHinder.h"
#include "Grid.h"
#include "Components/BoxComponent.h"

// Sets default values
AGridModifierWallHinder::AGridModifierWallHinder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(FName("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	boxCollision1 = CreateDefaultSubobject<UBoxComponent>("boxCollision1");
	boxCollision1->SetupAttachment(DefaultSceneRoot);

	boxCollision2 = CreateDefaultSubobject<UBoxComponent>("boxCollision2");
	boxCollision2->SetupAttachment(DefaultSceneRoot);

	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);

}

// Called when the game starts or when spawned
void AGridModifierWallHinder::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(modifierTimer, this, &AGridModifierWallHinder::ActivateCollision, 0.5, false);
	OnActorBeginOverlap.AddDynamic(this, &AGridModifierWallHinder::OnOverlap);
}

void AGridModifierWallHinder::ActivateCollision()
{
	SetActorEnableCollision(true);
}

void AGridModifierWallHinder::OnOverlap(AActor* me, AActor* other)
{
	AGrid* grid = Cast<AGrid>(other);
	if (grid != nullptr) {
		FIntPoint index1 = grid->GetTileIndexFromLocation(boxCollision1->GetComponentLocation());
		FIntPoint index2 = grid->GetTileIndexFromLocation(boxCollision2->GetComponentLocation());
		grid->gridTiles[index1].wallsHinder.Add(index2);
		grid->gridTiles[index2].wallsHinder.Add(index1);
		Destroy();
	}
}

