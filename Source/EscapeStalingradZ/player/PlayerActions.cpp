// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActions.h"
#include "Kismet/GameplayStatics.h"
#include "EscapeStalingradZ/Grid/Grid.h"
#include "actions/Command.h"
#include "actions/ActionMovementForward.h"
#include "Particles/ParticleSystemComponent.h"

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
    if (grid != nullptr && command != nullptr) {
        UpdateHoveredTile();
    }
}

void APlayerActions::LeftMouseClick()
{
    if (grid != nullptr) {
        UpdateHoveredTile();
        selectedTile = hoveredTile;
        if (command != nullptr) {
            command->Action(grid, actionTile, selectedTile);
            command = nullptr;
        }
    }
}

void APlayerActions::RightMouseClick()
{
    if (grid != nullptr) {
        selectedTile = FIntPoint(-1, -1);
    }
}

void APlayerActions::UpdateHoveredTile()
{
    FIntPoint index = grid->GetTileIndexUnderCursor();
    if (index.X < 0 || index.Y < 0) {
        index = FIntPoint(-1, -1);
    }
    if (index != hoveredTile) {
        if (grid->particleLoF->IsVisible()) {
            grid->DeActivateParticleLoF();
        }
        grid->RemoveTileState(hoveredTile, TileState::Hovered);
        hoveredTile = index;
        if (index != FIntPoint(-1, -1) && grid->gridTiles[index].states.Contains(TileState::isReachable)) {
            grid->AddTileState(index, TileState::Hovered);
            if (command != nullptr && command->NeedLoF()) {
                grid->SetParticleLoF(actionTile, index);
            }
        }
    }
}

