// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionNormalFire.h"
#include "EscapeStalingradZ/grid/Grid.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"

void UActionNormalFire::Execute(AGrid* grid, APlayerCharacter* character)
{
	if (grid != nullptr && character != nullptr) {
		FIntPoint indice = grid->GetTileIndexFromLocation(character->GetActorLocation());
		grid->AddTileState(indice, TileState::Selected);
		TArray<FIntPoint> list = TArray<FIntPoint>();
		int d = 100000;
		for (auto iter = character->LoFs.begin(); iter != character->LoFs.end(); ++iter) {
			if (list.Num() == 0) {
				list.Add(iter.Key());
				d = iter.Value().distance;
			}
			else {
				if (d == iter.Value().distance) {
					list.Add(iter.Key());
				}
				else if (d > iter.Value().distance) {
					list.Empty();
					list.Add(iter.Key());
					d = iter.Value().distance;
				}
			}
			
		}
		for (FIntPoint index : list) {
			grid->AddTileState(index, TileState::isReachable);
		}
	}
}
void UActionNormalFire::Action(AGrid* grid, FIntPoint tile, FIntPoint destinyTile)
{
	if (grid != nullptr && tile != FIntPoint(-1, -1)) {
		grid->RemoveTileState(tile, TileState::Selected);
		APlayerCharacter* character = Cast<APlayerCharacter>(grid->gridTiles[tile].actor);
		if (character != nullptr) {
			if (destinyTile != FIntPoint(-1, -1)) {
				if (grid->gridTiles[destinyTile].states.Contains(TileState::isReachable)) {
					grid->RemoveTileState(destinyTile, TileState::Hovered);
					grid->DeActivateParticleLoF();
					//TO-DO
				}
			}
			
			for (auto iter = character->LoFs.begin(); iter != character->LoFs.end(); ++iter) {
				grid->RemoveTileState(iter.Key(), TileState::isReachable);
			}
		}
	}

}

bool UActionNormalFire::NeedLoF()
{
	return true;
}
