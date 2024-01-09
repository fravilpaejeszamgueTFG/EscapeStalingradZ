// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionMovementForward.h"
#include "EscapeStalingradZ/grid/Grid.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"


void UActionMovementForward::Execute(AGrid* grid, APlayerCharacter* character)
{
	if (grid != nullptr && character != nullptr) {
		FIntPoint indice = grid->GetTileIndexFromLocation(character->GetActorLocation());
		grid->AddTileState(indice, TileState::Selected);
		int numCasillas = character->mp;
		TArray<FIntPoint> indices = grid->GetTilesForward(indice, character->GetActorForwardVector(), numCasillas);
		for (FIntPoint l : indices) {
			grid->AddTileState(l, TileState::isReachable);
		}
	}
	
}

void UActionMovementForward::Action(AGrid* grid, FIntPoint tile, FIntPoint destinyTile)
{
	if (grid != nullptr && tile != FIntPoint(-1,-1)) {
		grid->RemoveTileState(tile, TileState::Selected);
		APlayerCharacter* character = Cast<APlayerCharacter>(grid->gridTiles[tile].actor);
		if (character != nullptr) {
			int numCasillas = character->mp;
			TArray<FIntPoint> indices = grid->GetTilesForward(tile, character->GetActorForwardVector(), numCasillas);
			if (destinyTile != FIntPoint(-1, -1)) {
				if (grid->gridTiles[destinyTile].states.Contains(TileState::isReachable)) {
					grid->RemoveTileState(destinyTile, TileState::Hovered);
					grid->gridTiles[tile].actor = nullptr;
					grid->gridTiles[destinyTile].actor = character;
					character->SetActorLocation(grid->GetLocationByIndex(destinyTile));
					for (int i = 0; i < indices.Num(); i++) {
						if (indices[i] == destinyTile) {
							character->mp -= i+1;
						}
					}

				}
			}
			for (FIntPoint i : indices) {
				grid->RemoveTileState(i, TileState::isReachable);
			}
		}
	}

}

