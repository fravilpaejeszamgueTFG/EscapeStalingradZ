// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSearch.h"
#include "EscapeStalingradZ/Grid/Grid.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/widget/UserHUD.h"
#include "EscapeStalingradZ/widget/WPlayerInfo.h"

void UActionSearch::Execute(AGrid* grid, APlayerCharacter* character)
{
	if (grid != nullptr && character != nullptr) {
		FIntPoint indice = grid->GetTileIndexFromLocation(character->GetActorLocation());
		FVector fv = character->GetActorForwardVector();
		FVector rv = character->GetActorRightVector();
		TArray<FIntPoint> indices = grid->GetFrontTiles(indice, fv, rv);
		for (FIntPoint l : indices) {
			if (grid->gridTiles[l].types.Contains(TileType::Search)) {
				grid->AddTileState(l, TileState::isReachable);
			}
		}
		if (grid->gridTiles[indice].types.Contains(TileType::Search)) {
			grid->AddTileState(indice, TileState::isReachable);
		}
	}
}

void UActionSearch::Action(AGrid* grid, FIntPoint tile, FIntPoint destinyTile)
{
	if (grid != nullptr && tile != FIntPoint(-1, -1)) {
		APlayerCharacter* character = Cast<APlayerCharacter>(grid->gridTiles[tile].actor);
		if (character != nullptr) {
			FVector fv = character->GetActorForwardVector();
			FVector rv = character->GetActorRightVector();
			TArray<FIntPoint> indices = grid->GetFrontTiles(tile, fv, rv);
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
					grid->currentSearchTile = destinyTile;
				}
			}
			grid->RemoveTileState(tile, TileState::isReachable);
			for (FIntPoint i : indices) {
				grid->RemoveTileState(i, TileState::isReachable);
			}
		}
	}
}
