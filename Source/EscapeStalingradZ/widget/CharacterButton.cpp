// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterButton.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/turn/Turn.h"
#include "buttons/Boton.h"
#include "EscapeStalingradZ/Grid/Grid.h"
#include "EscapeStalingradZ/player/PlayerActions.h"
#include "Kismet/GameplayStatics.h"
#include "EscapeStalingradZ/player/PlayerC.h"
#include "WSelectCharacterTurn.h"
#include "WSelectMovementType.h"

UCharacterButton::UCharacterButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCharacterButton::NativeConstruct()
{
	Super::NativeConstruct();

	buttonChara->OnClicked.AddDynamic(this, &UCharacterButton::OnClick);
	buttonChara->OnHovered.AddDynamic(this, &UCharacterButton::OnHover);
	buttonChara->OnUnhovered.AddDynamic(this, &UCharacterButton::OnUnHover);

}

void UCharacterButton::OnClick()
{
	if (grid != nullptr && tile != FIntPoint(-1, -1)) {
		grid->RemoveTileState(tile, TileState::Hovered);
		grid->AddTileState(tile, TileState::Selected);
		APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (player != nullptr && player->actions!=nullptr) {
			player->actions->actionTile = tile;
			player->playerchara = character;
			turn->selectedCharacter = character;
			player->SetMovementWidget();
			player->Movement->turn = turn;
		}
		character->attacked = false;
		selectCharacter->onClickButton();
	}
}

void UCharacterButton::OnHover()
{
	if (character != nullptr) {
		grid = character->grid;
		if (grid != nullptr) {
			tile = grid->GetTileIndexFromLocation(character->GetActorLocation());
			grid->AddTileState(tile, TileState::Hovered);
		}
	}
	
}

void UCharacterButton::OnUnHover()
{
	if (grid != nullptr && tile !=FIntPoint(-1,-1)) {
		grid->RemoveTileState(tile, TileState::Hovered);
	}
}
