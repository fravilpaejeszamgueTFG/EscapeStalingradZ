// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSelectTileToMove.h"
#include "EscapeStalingradZ/grid/Grid.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "EscapeStalingradZ/player/PlayerActions.h"
#include "EscapeStalingradZ/player/PlayerC.h"
#include "EscapeStalingradZ/turn/Turn.h"

void UActionSelectTileToMove::Execute(AGrid* grid, APlayerCharacter* character)
{
	if (grid != nullptr && character != nullptr) {
		FIntPoint indice = grid->GetTileIndexFromLocation(character->GetActorLocation());
		TArray<FIntPoint> indices = grid->GetTileNeighborsThatCanMoveInto(indice);
		if (indices.Num() == 0) {
			UE_LOG(LogTemp, Warning, TEXT("Fin partida, personaje muere"));
		}
		for (FIntPoint l : indices) {
			grid->AddTileState(l, TileState::isReachable);
		}
	}
}

void UActionSelectTileToMove::Action(AGrid* grid, FIntPoint tile, FIntPoint destinyTile)
{
	if (grid != nullptr && tile != FIntPoint(-1, -1)) {
		APlayerCharacter* character = Cast<APlayerCharacter>(grid->gridTiles[tile].actor);
		if (character != nullptr) {
			if (destinyTile != FIntPoint(-1, -1)) {
				if (grid->gridTiles[destinyTile].states.Contains(TileState::isReachable)) {
					grid->RemoveTileState(destinyTile, TileState::Hovered);
					TArray<FIntPoint> indices = grid->GetTileNeighborsThatCanMoveInto(tile);
					grid->gridTiles[tile].actor = nullptr;
					grid->gridTiles[destinyTile].actor = character;
					character->SetActorLocation(grid->GetLocationByIndex(destinyTile));
					for (FIntPoint i : indices) {
						grid->RemoveTileState(i, TileState::isReachable);
					}
					turn->SpawnZombieAfterMoveCharacter(zombie);
				}
				else {
					APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
					if (player != nullptr) {
						player->actions->deleteCommand = false;
					}
				}
			}
			else {
				APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
				if (player != nullptr) {
					player->actions->deleteCommand = false;
				}
			}
		}
	}
}
