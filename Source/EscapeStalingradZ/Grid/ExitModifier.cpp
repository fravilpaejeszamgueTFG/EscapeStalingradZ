// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitModifier.h"
#include "Grid.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "EscapeStalingradZ/player/PlayerC.h"

// Sets default values
AExitModifier::AExitModifier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(FName("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	boxCollision = CreateDefaultSubobject<UBoxComponent>("boxCollision");
	boxCollision->SetupAttachment(DefaultSceneRoot);

	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);

}

// Called when the game starts or when spawned
void AExitModifier::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(modifierTimer, this, &AExitModifier::ActivateCollision, 0.6, false);
	OnActorBeginOverlap.AddDynamic(this, &AExitModifier::OnOverlap);
	
}

void AExitModifier::ActivateCollision()
{
	APlayerC* player = Cast<APlayerC>(GetWorld()->GetFirstPlayerController());
	if (player != nullptr && !player->canExitTheRoom) {
		player->exits.Add(this);
	}
	else {
		SetActorEnableCollision(true);
	}
}

void AExitModifier::OnOverlap(AActor* me, AActor* other)
{
	AGrid* grid = Cast<AGrid>(other);
	if (grid != nullptr) {
		FIntPoint index = grid->GetTileIndexFromLocation(GetActorLocation());
		grid->gridTiles[index].types.Remove(TileType::Normal);
		grid->gridTiles[index].types.Add(type);
		Destroy();
	}
}


