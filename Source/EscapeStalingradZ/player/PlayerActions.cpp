// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActions.h"
#include "Kismet/GameplayStatics.h"
#include "EscapeStalingradZ/Grid/Grid.h"
#include "actions/Command.h"
#include "actions/ActionMovementForward.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
UPlayerActions::UPlayerActions()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void UPlayerActions::BeginPlay()
{
    Super::BeginPlay();
}

void UPlayerActions::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    if (grid != nullptr && command != nullptr) {
        UpdateHoveredTile();
    }
}

void UPlayerActions::LeftMouseClick()
{
    if (grid != nullptr && command != nullptr) {
        UpdateHoveredTile();
        selectedTile = hoveredTile;
        command->Action(grid, actionTile, selectedTile);
        if (deleteCommand) {
            command = nullptr;
        }
        else {
            deleteCommand = true;
        }
            
    }
}

void UPlayerActions::RightMouseClick()
{
    if (grid != nullptr) {
        selectedTile = FIntPoint(-1, -1);
    }
}

void UPlayerActions::UpdateHoveredTile()
{
    FIntPoint index = grid->GetTileIndexUnderCursor();
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

