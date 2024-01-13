// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionMovementLateral.h"
#include "EscapeStalingradZ/grid/Grid.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"

void UActionMovementLateral::Execute(AGrid* grid, APlayerCharacter* character)
{
	if (grid != nullptr && character != nullptr) {
		FIntPoint indice = grid->GetTileIndexFromLocation(character->GetActorLocation());
		grid->AddTileState(indice, TileState::Selected);
		int numCasillas = (character->mp)/2;
		TArray<FIntPoint> indices = grid->GetTilesForwardMovement(indice, character->GetActorRightVector(), numCasillas, character->mp, 2);
		indices.Append(grid->GetTilesForwardMovement(indice, -(character->GetActorRightVector()), numCasillas, character->mp, 2));
		for (FIntPoint l : indices) {
			grid->AddTileState(l, TileState::isReachable);
		}
	}
}

void UActionMovementLateral::Action(AGrid* grid, FIntPoint tile, FIntPoint destinyTile)
{
	if (grid != nullptr && tile != FIntPoint(-1, -1)) {
		grid->RemoveTileState(tile, TileState::Selected);
		APlayerCharacter* character = Cast<APlayerCharacter>(grid->gridTiles[tile].actor);
		if (character != nullptr) {
			int numCasillas = (character->mp)/2;
			TArray<FIntPoint> indicesright = grid->GetTilesForwardMovement(tile, character->GetActorRightVector(), numCasillas, character->mp, 2);
			TArray<FIntPoint> indicesleft = grid->GetTilesForwardMovement(tile, -(character->GetActorRightVector()), numCasillas, character->mp, 2);
			if (destinyTile != FIntPoint(-1, -1)) {
				if (grid->gridTiles[destinyTile].states.Contains(TileState::isReachable)) {
					grid->RemoveTileState(destinyTile, TileState::Hovered);
					grid->gridTiles[tile].actor = nullptr;
					grid->gridTiles[destinyTile].actor = character;
					character->SetActorLocation(grid->GetLocationByIndex(destinyTile));
					int cont = 0;
					bool tileRight = false;
					for (int i = 0; i < indicesright.Num(); i++) {
						if (grid->gridTiles[indicesright[i]].types.Contains(TileType::Hinder)) {
							cont++;
						}
						if (indicesright[i] == destinyTile) {
							character->mp -= (i + 1) * 2;
							tileRight = true;
							break;
						}
					}
					if (!tileRight) {
						cont = 0;
						for (int i = 0; i < indicesleft.Num(); i++) {
							if (grid->gridTiles[indicesleft[i]].types.Contains(TileType::Hinder)) {
								cont++;
							}
							if (indicesleft[i] == destinyTile) {
								character->mp -= (i + 1) * 2;
								break;
							}
						}
					}
					character->mp -= cont;
				}
			}
			for (FIntPoint i : indicesright) {
				grid->RemoveTileState(i, TileState::isReachable);
			}
			for (FIntPoint i : indicesleft) {
				grid->RemoveTileState(i, TileState::isReachable);
			}
		}
	}
}
