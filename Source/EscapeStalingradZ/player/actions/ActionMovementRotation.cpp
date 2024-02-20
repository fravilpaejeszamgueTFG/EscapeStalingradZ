// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionMovementRotation.h"
#include "EscapeStalingradZ/grid/Grid.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/widget/UserHUD.h"
#include "EscapeStalingradZ/widget/WPlayerInfo.h"

void UActionMovementRotation::Execute(AGrid* grid, APlayerCharacter* character)
{
	if (grid != nullptr && character != nullptr) {
		FIntPoint indice = grid->GetTileIndexFromLocation(character->GetActorLocation());
		int mp = character->mp;
		if (character->typeOfMovement == MovementType::Running) {
			mp = (character->mp)/2;
		}
		FVector forwardVector = character->GetActorForwardVector();
		FVector rightVector = character->GetActorRightVector();
		TArray<FIntPoint> indices = grid->GetTilesRotation(indice, forwardVector, rightVector, mp);
		for (FIntPoint l : indices) {
			grid->AddTileState(l, TileState::isReachable);
		}
	}
}

void UActionMovementRotation::Action(AGrid* grid, FIntPoint tile, FIntPoint destinyTile)
{
	if (grid != nullptr && tile != FIntPoint(-1, -1)) {
		APlayerCharacter* character = Cast<APlayerCharacter>(grid->gridTiles[tile].actor);
		if (character != nullptr) {
			int mp = character->mp;
			if (character->typeOfMovement == MovementType::Running) {
				mp = (character->mp) / 2;
			}
			FVector forwardVector = character->GetActorForwardVector();
			FVector rightVector = character->GetActorRightVector();
			TArray<FIntPoint> indices = grid->GetTilesRotation(tile, forwardVector, rightVector, mp);
			if (destinyTile != FIntPoint(-1, -1)) {
				if (grid->gridTiles[destinyTile].states.Contains(TileState::isReachable)) {
					grid->RemoveTileState(destinyTile, TileState::Hovered);
					FRotator rotation = GetRotationDirection(tile, destinyTile);
					character->SetActorRotation(rotation);
					CheckMP(character, forwardVector, character->GetActorForwardVector());
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

void UActionMovementRotation::CheckMP(APlayerCharacter* character, FVector oldFV, FVector newFV)
{
	int cost = 1;
	if (character->typeOfMovement == MovementType::Running) {
		cost = 2;
	}
	if (round(oldFV.X) == -round(newFV.X) || round(oldFV.Y) == -round(newFV.Y)) {
		character-> mp -= 2 * cost;
	}
	else {
		character-> mp-= cost;
	}
}

FRotator UActionMovementRotation::GetRotationDirection(FIntPoint tile, FIntPoint destinyTile)
{
	if (destinyTile.X > tile.X) {
		return FRotator(0, 0, 0);
	}
	else if (destinyTile.X < tile.X) {
		return FRotator(0, 180, 0);
	}
	else if (destinyTile.Y > tile.Y) {
		return FRotator(0, 90, 0);
	}
	else {
		return FRotator(0, 270, 0);
	}
}

