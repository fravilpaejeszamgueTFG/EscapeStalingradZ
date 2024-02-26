// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionNewObjectiveSpreadFire.h"
#include "EscapeStalingradZ/grid/Grid.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "EscapeStalingradZ/player/PlayerActions.h"
#include "EscapeStalingradZ/player/PlayerC.h"
#include "EscapeStalingradZ/widget/WDiceSpreadCombat.h"

void UActionNewObjectiveSpreadFire::Execute(AGrid* grid, APlayerCharacter* character)
{
	if (grid != nullptr && character != nullptr) {
		TArray<FIntPoint> list = TArray<FIntPoint>();
		list.Append(grid->GetTileNeighbors(tileZombie));
		TMap<FIntPoint, int> tileWithDistance = TMap<FIntPoint, int>();
		bool tileisReachable = false;
		for (auto& i : character->LoFs) {
			FIntPoint tile = i.Key;
			if (tile != tileZombie && grid->gridTiles[tile].actor != nullptr) {
				int distance = abs(tile.X - tileZombie.X) + abs(tile.Y - tileZombie.Y);
				if (distance <= NumberDiceLeft) {
					tileWithDistance.Add(tile, distance);
				}
			}
		}
		int number;
		for (FIntPoint index : list) {
			number = grid->GetCostToEnterNeighbor(tileZombie, index);
			if (number > 0) {
				for (auto& i : tileWithDistance) {
					FIntPoint tile = i.Key;
					int distance = i.Value;
					int newDistance = abs(tile.X - index.X) + abs(tile.Y - index.Y);
					if (newDistance <= distance) {
						grid->AddTileState(index, TileState::isReachable);
						tileisReachable = true;
						break;
					}
				}
			}
		}
		if (grid->gridTiles[tileZombie].actor != nullptr) {
			tileisReachable = true;
			grid->AddTileState(tileZombie, TileState::isReachable);
		}
		if (!tileisReachable) {
			character->DiceSpreadCombatWidget->EndAttack();
			APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if (player != nullptr) {
				player->actions->command = nullptr;
			}
		}
	}
}

void UActionNewObjectiveSpreadFire::Action(AGrid* grid, FIntPoint tile, FIntPoint destinyTile)
{
	if (grid != nullptr && tile != FIntPoint(-1, -1)) {
		APlayerCharacter* character = Cast<APlayerCharacter>(grid->gridTiles[tile].actor);
		if (character != nullptr) {
			if (destinyTile != FIntPoint(-1, -1)) {
				if (grid->gridTiles[destinyTile].states.Contains(TileState::isReachable)) {
					grid->RemoveTileState(destinyTile, TileState::Hovered);
					AZombie* zombie = Cast<AZombie>(grid->gridTiles[destinyTile].actor);
					if (zombie != nullptr) {
						character->MoveToTileWithZombieDuringSpreadFire(zombie, destinyTile);
					}
					else {
						character->MoveToTileWithoutZombieDuringSpreadFire(destinyTile);
					}
					for (auto& t : grid->gridTiles) {
						grid->RemoveTileState(t.Key, TileState::isReachable);
					}
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

