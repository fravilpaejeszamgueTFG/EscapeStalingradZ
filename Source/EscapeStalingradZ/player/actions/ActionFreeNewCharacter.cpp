// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionFreeNewCharacter.h"
#include "EscapeStalingradZ/grid/Grid.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/character/CharacterToFree.h"
#include "EscapeStalingradZ/player/PlayerC.h"
#include "EscapeStalingradZ/widget/UserHUD.h"
#include "EscapeStalingradZ/widget/WPlayerInfo.h"

void UActionFreeNewCharacter::Execute(AGrid* grid, APlayerCharacter* character)
{
	if (grid != nullptr && character != nullptr) {
		FIntPoint indice = grid->GetTileIndexFromLocation(character->GetActorLocation());
		FVector fv = character->GetActorForwardVector();
		FVector rv = character->GetActorRightVector();
		TArray<FIntPoint> indices = grid->GetFrontTiles(indice, fv, rv);
		for (FIntPoint l : indices) {
			ACharacterToFree* chara = Cast<ACharacterToFree>(grid->gridTiles[l].actor);
			if (chara != nullptr) {
				grid->AddTileState(l, TileState::isReachable);
			}
		}
	}
}

void UActionFreeNewCharacter::Action(AGrid* grid, FIntPoint tile, FIntPoint destinyTile)
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
					ACharacterToFree* chara = Cast<ACharacterToFree>(grid->gridTiles[destinyTile].actor);
					if (chara != nullptr) {
						character->attacked = true;
						character->SpawnNewCharacter(chara->characterChosen);
						chara->Destroy();
						APlayerC* player = Cast<APlayerC>(GetWorld()->GetFirstPlayerController());
						if (player != nullptr && GetWorld()->GetName() == "AFriendWillBleed") {
							player->CompletedPrimaryObjective();
						}
						AUserHUD* hud = Cast<AUserHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
						if (hud != nullptr && hud->PlayerInfoWidget != nullptr) {
							hud->PlayerInfoWidget->SetVisibleActionWidget();
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

