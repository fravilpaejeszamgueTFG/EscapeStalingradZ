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
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

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
		character->attacked = false;
		character->typeOfCovering = CoveringType::NONE;
		selectCharacter->onClickButton();
		turn->selectedCharacter = character;
		if (character->isLocked) {
			FTimerHandle prueba;
			GetWorld()->GetTimerManager().SetTimer(prueba, turn, &ATurn::nextCharacter, 1.f, false);
			//Liberacion fijado
		}
		else {
			grid->AddTileState(tile, TileState::Selected);
			APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if (player != nullptr && player->actions != nullptr) {
				player->actions->actionTile = tile;
				player->playerchara = character;
				player->SetMovementWidget();
				player->Movement->turn = turn;
			}
		}
	}
}

void UCharacterButton::OnHover()
{
	if (character != nullptr) {
		if (grid == nullptr) {
			grid = character->grid;
		}
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
