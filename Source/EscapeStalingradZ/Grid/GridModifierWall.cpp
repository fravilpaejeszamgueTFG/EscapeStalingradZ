// Fill out your copyright notice in the Description page of Project Settings.


#include "GridModifierWall.h"
#include "Grid.h"
#include "Components/BoxComponent.h"

// Sets default values
AGridModifierWall::AGridModifierWall()
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
void AGridModifierWall::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(modifierTimer, this, &AGridModifierWall::ActivateCollision, 0.5, false);
	OnActorBeginOverlap.AddDynamic(this, &AGridModifierWall::OnOverlap);
	
}

void AGridModifierWall::ActivateCollision()
{
	SetActorEnableCollision(true);
}

void AGridModifierWall::OnOverlap(AActor* me, AActor* other)
{
	AGrid* grid = Cast<AGrid>(other);
	if (grid != nullptr) {
		FIntPoint index1 = grid->GetTileIndexFromLocation(boxCollision1->GetComponentLocation());
		FIntPoint index2 = grid->GetTileIndexFromLocation(boxCollision2->GetComponentLocation());
		grid->gridTiles[index1].walls.Add(index2);
		grid->gridTiles[index2].walls.Add(index1);
		Destroy();
	}
}

