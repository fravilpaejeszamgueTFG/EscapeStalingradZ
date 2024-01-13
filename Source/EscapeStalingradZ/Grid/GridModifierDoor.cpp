// Fill out your copyright notice in the Description page of Project Settings.


#include "GridModifierDoor.h"
#include "Grid.h"
#include "Components/BoxComponent.h"

// Sets default values
AGridModifierDoor::AGridModifierDoor()
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
void AGridModifierDoor::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(modifierTimer, this, &AGridModifierDoor::ActivateCollision, 0.5, false);
	OnActorBeginOverlap.AddDynamic(this, &AGridModifierDoor::OnOverlap);

}

void AGridModifierDoor::ActivateCollision()
{
	SetActorEnableCollision(true);
}

void AGridModifierDoor::OnOverlap(AActor* me, AActor* other)
{
	AGrid* grid = Cast<AGrid>(other);
	if (grid != nullptr) {
		FIntPoint index1 = grid->GetTileIndexFromLocation(boxCollision1->GetComponentLocation());
		FIntPoint index2 = grid->GetTileIndexFromLocation(boxCollision2->GetComponentLocation());
		if (grid->gridTiles[index1].doors.Contains(index2)) {
			grid->gridTiles[index1].doors[index2] = true;
		}
		else {
			grid->gridTiles[index1].doors.Add(index2, true);
		}
		if (grid->gridTiles[index2].doors.Contains(index1)) {
			grid->gridTiles[index2].doors[index1] = true;
		}
		else {
			grid->gridTiles[index2].doors.Add(index1, true);
		}
		Destroy();
	}
}


