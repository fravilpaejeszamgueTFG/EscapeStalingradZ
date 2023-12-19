// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActions.h"
#include "Kismet/GameplayStatics.h"
#include "EscapeStalingradZ/Grid/Grid.h"

// Sets default values
APlayerActions::APlayerActions()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerActions::BeginPlay()
{
	Super::BeginPlay();
    EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &APlayerActions::LeftMouseClick);
    InputComponent->BindAction("RightMouseButton", IE_Pressed, this, &APlayerActions::RightMouseClick);
}

void APlayerActions::Tick(float DeltaTime)
{
    if (grid != nullptr) {
        UpdateHoveredTile();
    }
}

void APlayerActions::LeftMouseClick()
{
    if (grid != nullptr) {
        if (selectedTile != FIntPoint(-1, -1)) {
            grid->RemoveTileState(selectedTile, TileState::Selected);
            grid->UpdateTileNeighbors(selectedTile, false);
        }
        UpdateHoveredTile();
        selectedTile = hoveredTile;
        grid->AddTileState(selectedTile, TileState::Selected);
        grid->UpdateTileNeighbors(selectedTile, true);
    }
}

void APlayerActions::RightMouseClick()
{
    if (grid != nullptr) {
        if (selectedTile != FIntPoint(-1, -1)) {
            grid->RemoveTileState(selectedTile, TileState::Selected);
            grid->UpdateTileNeighbors(selectedTile, false);
        }
        selectedTile = FIntPoint(-1, -1);
    }
}

void APlayerActions::UpdateHoveredTile()
{
    FIntPoint index = grid->GetTileIndexUnderCursor();
    if (index != hoveredTile) {
        grid->RemoveTileState(hoveredTile, TileState::Hovered);
        grid->AddTileState(index, TileState::Hovered);
        hoveredTile = index;
    }
}

