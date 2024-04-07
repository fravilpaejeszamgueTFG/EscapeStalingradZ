// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionMovementBackward.h"
#include "EscapeStalingradZ/grid/Grid.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/zombies/Zombie.h"
#include "EscapeStalingradZ/widget/UserHUD.h"
#include "EscapeStalingradZ/widget/WPlayerInfo.h"

void UActionMovementBackward::Execute(AGrid* grid, APlayerCharacter* character)
{
	if (grid != nullptr && character != nullptr) {
		FIntPoint indice = grid->GetTileIndexFromLocation(character->GetActorLocation());
		int numCasillas = (character->mp)/2;
		TArray<FIntPoint> indices = grid->GetTilesForwardMovement(indice, -(character->GetActorForwardVector()), numCasillas, character->mp, 2);
		for (FIntPoint l : indices) {
			grid->AddTileState(l, TileState::isReachable);
			if (grid->HasZombieInNeighbor(l)) {
				break;
			}
		}
	}
}

void UActionMovementBackward::Action(AGrid* grid, FIntPoint tile, FIntPoint destinyTile)
{
	if (grid != nullptr && tile != FIntPoint(-1, -1)) {
		APlayerCharacter* character = Cast<APlayerCharacter>(grid->gridTiles[tile].actor);
		if (character != nullptr) {
			int numCasillas = (character->mp)/2;
			TArray<FIntPoint> indices = grid->GetTilesForwardMovement(tile, -(character->GetActorForwardVector()), numCasillas, character->mp, 2);
			if (destinyTile != FIntPoint(-1, -1)) {
				if (grid->gridTiles[destinyTile].states.Contains(TileState::isReachable)) {
					grid->RemoveTileState(destinyTile, TileState::Hovered);
					grid->RemoveTileState(tile, TileState::Selected);
					grid->AddTileState(destinyTile, TileState::Selected);
					grid->gridTiles[tile].actor = nullptr;
					grid->gridTiles[destinyTile].actor = character;
					character->SetActorLocation(grid->GetLocationByIndex(destinyTile));
					for (int i = 0; i < indices.Num(); i++) {
						if (grid->gridTiles[indices[i]].types.Contains(TileType::Hinder)) {
							character->mp--;
						}
						if (indices[i] == destinyTile) {
							character->mp -= (i + 1) * 2;
							break;
						}
					}
					AZombie* zombie = grid->ZombieInNeighbor(destinyTile);
					if (zombie != nullptr) {
						character->inDirectContact = true;
						zombie->characterInContact = character;
					}
					AUserHUD* hud = Cast<AUserHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
					if (hud != nullptr && hud->PlayerInfoWidget != nullptr) {
						hud->PlayerInfoWidget->SetVisibleActionWidget();
					}
					grid->EndIfTileIsExit(destinyTile);
				}
			}
			for (FIntPoint i : indices) {
				grid->RemoveTileState(i, TileState::isReachable);
			}
		}
	}
}
