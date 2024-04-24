// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionExchangeEquipment.h"
#include "EscapeStalingradZ/grid/Grid.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/widget/UserHUD.h"

void UActionExchangeEquipment::Execute(AGrid* grid, APlayerCharacter* character)
{
	if (grid != nullptr && character != nullptr) {
		FIntPoint indice = grid->GetTileIndexFromLocation(character->GetActorLocation());
		FVector fv = character->GetActorForwardVector();
		FVector rv = character->GetActorRightVector();
		TArray<FIntPoint> indices = grid->GetFrontTiles(indice, fv, rv);
		for (FIntPoint l : indices) {
			APlayerCharacter* chara = Cast<APlayerCharacter>(grid->gridTiles[l].actor);
			if (chara != nullptr) {
				grid->AddTileState(l, TileState::isReachable);
			}
		}
	}
}

void UActionExchangeEquipment::Action(AGrid* grid, FIntPoint tile, FIntPoint destinyTile)
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
					APlayerCharacter* chara = Cast<APlayerCharacter>(grid->gridTiles[destinyTile].actor);
					if (chara != nullptr) {
						character->mp -= 2;
						AUserHUD* hud = Cast<AUserHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
						if (hud != nullptr) {
							hud->CreateOrSetExchangeEquipment(character, chara);
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
