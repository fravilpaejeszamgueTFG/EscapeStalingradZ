// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSearch.h"
#include "EscapeStalingradZ/grid/Grid.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/widget/UserHUD.h"
#include "EscapeStalingradZ/widget/WPlayerInfo.h"

void UActionSearch::Execute(AGrid* grid, APlayerCharacter* character)
{
	if (grid != nullptr && character != nullptr) {
		FIntPoint indice = grid->GetTileIndexFromLocation(character->GetActorLocation());
		TArray<FIntPoint> neighbors = grid->GetTileNeighbors(indice);
		for (FIntPoint index : neighbors) {
			if (grid->gridTiles[index].types.Contains(TileType::Search)) {
				grid->AddTileState(index, TileState::isReachable);
			}
		}
	}
}

void UActionSearch::Action(AGrid* grid, FIntPoint tile, FIntPoint destinyTile)
{
	if (grid != nullptr && tile != FIntPoint(-1, -1)) {
		APlayerCharacter* character = Cast<APlayerCharacter>(grid->gridTiles[tile].actor);
		if (character != nullptr) {
			TArray<FIntPoint> neighbors = grid->GetTileNeighbors(tile);
			if (destinyTile != FIntPoint(-1, -1)) {
				if (grid->gridTiles[destinyTile].states.Contains(TileState::isReachable)) {
					grid->RemoveTileState(destinyTile, TileState::Hovered);
					character->attacked = true;
					character->mp = 0;
					AUserHUD* hud = Cast<AUserHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
					if (hud != nullptr && hud->PlayerInfoWidget != nullptr) {
						hud->PlayerInfoWidget->HidePlayerInfoDuringSearch();
					}
					character->SearchAction();
					grid->gridTiles[destinyTile].types.Remove(TileType::Search);
				}
			}
			for (FIntPoint i : neighbors) {
				grid->RemoveTileState(i, TileState::isReachable);
			}
		}
	}
}
