// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionOpenCloseDoor.h"
#include "EscapeStalingradZ/grid/Grid.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/widget/UserHUD.h"
#include "EscapeStalingradZ/widget/WPlayerInfo.h"

void UActionOpenCloseDoor::Execute(AGrid* grid, APlayerCharacter* character)
{
	if (grid != nullptr && character != nullptr) {
		FIntPoint indice = grid->GetTileIndexFromLocation(character->GetActorLocation());
		int mp = character->mp;
		if (character->typeOfMovement == MovementType::Running) {
			mp = (character->mp) / 2;
		}
		if (mp >= 1 && grid->gridTiles[indice].doors.Num() > 0) {
			FVector fv = character->GetActorForwardVector();
			FVector rv = character->GetActorRightVector();
			TArray<FIntPoint> indices = grid->GetFrontTiles(indice, fv, rv);
			for (FIntPoint l : indices) {
				if (grid->gridTiles[indice].doors.Contains(l)) {
					grid->AddTileState(l, TileState::isReachable);
				}
			}
		}
	}
}

void UActionOpenCloseDoor::Action(AGrid* grid, FIntPoint tile, FIntPoint destinyTile)
{
	if (grid != nullptr && tile != FIntPoint(-1, -1)) {
		APlayerCharacter* character = Cast<APlayerCharacter>(grid->gridTiles[tile].actor);
		if (character != nullptr) {
			FVector fv = character->GetActorForwardVector();
			FVector rv = character->GetActorRightVector();
			TArray<FIntPoint> indices = grid->GetFrontTiles(tile, fv, rv);
			if (destinyTile != FIntPoint(-1, -1)) {
				if (grid->gridTiles[destinyTile].states.Contains(TileState::isReachable)) {
					bool door = grid->gridTiles[tile].doors[destinyTile];
					grid->RemoveTileState(destinyTile, TileState::Hovered);
					grid->gridTiles[tile].doors[destinyTile] = !door;
					grid->gridTiles[destinyTile].doors[tile] = !door;
					if (character->typeOfMovement == MovementType::Running) {
						character->mp -=2;
					}
					else {
						character->mp--;
					}
					AUserHUD* hud = Cast<AUserHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
					if (hud != nullptr && hud->PlayerInfoWidget != nullptr) {
						hud->PlayerInfoWidget->SetVisibleActionWidget();
					}
				}
			}
			for (FIntPoint i : indices) {
				grid->RemoveTileState(i, TileState::isReachable);
			}
		}
	}
}
