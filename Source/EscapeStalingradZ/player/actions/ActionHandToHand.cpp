// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionHandToHand.h"
#include "EscapeStalingradZ/grid/Grid.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/zombies/Zombie.h"
#include "EscapeStalingradZ/player/PlayerActions.h"

void UActionHandToHand::Execute(AGrid* grid, APlayerCharacter* character)
{
	if (grid != nullptr && character != nullptr) {
		FIntPoint indice = grid->GetTileIndexFromLocation(character->GetActorLocation());
		TArray<FIntPoint> indices;
		if (character->GetDistanceAttackHandToHand() == 1) {
			indices = grid->GetTileNeighbors(indice);
		}
		else {
			indices = character->GetIndexHandToHand2Range();
		}
		for (FIntPoint i : indices) {
			AZombie* zombie = Cast<AZombie>(grid->gridTiles[i].actor);
			if (zombie != nullptr) {
				grid->AddTileState(i, TileState::isReachable);
			}
		}
	}
}
void UActionHandToHand::Action(AGrid* grid, FIntPoint tile, FIntPoint destinyTile)
{
	if (grid != nullptr && tile != FIntPoint(-1, -1)) {
		APlayerCharacter* character = Cast<APlayerCharacter>(grid->gridTiles[tile].actor);
		if (character != nullptr) {
			if (destinyTile != FIntPoint(-1, -1)) {
				if (grid->gridTiles[destinyTile].states.Contains(TileState::isReachable)) {
					grid->RemoveTileState(destinyTile, TileState::Hovered);
					AZombie* zombie = Cast<AZombie>(grid->gridTiles[destinyTile].actor);
					if (zombie != nullptr) {
						character->AttackZombieHandToHand(zombie, destinyTile);
					}
				}
			}
			TArray<FIntPoint> indices;
			if (character->GetDistanceAttackHandToHand() == 1) {
				indices = grid->GetTileNeighbors(tile);
			}
			else {
				indices = character->GetIndexHandToHand2Range();
			}
			for (FIntPoint i : indices) {
				AZombie* zombie = Cast<AZombie>(grid->gridTiles[i].actor);
				if (zombie != nullptr) {
					grid->RemoveTileState(i, TileState::isReachable);
				}
			}
		}
	}
}
