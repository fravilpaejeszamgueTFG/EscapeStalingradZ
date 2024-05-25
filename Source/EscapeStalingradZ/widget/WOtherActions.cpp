// Fill out your copyright notice in the Description page of Project Settings.


#include "WOtherActions.h"
#include "WActions.h"
#include "buttons/Boton.h"
#include "EscapeStalingradZ/Grid/Grid.h"
#include "EscapeStalingradZ/turn/Turn.h"
#include "EscapeStalingradZ/player/actions/ActionFreeNewCharacter.h"
#include "EscapeStalingradZ/player/actions/ActionMovementRotation.h"
#include "EscapeStalingradZ/player/actions/ActionOpenCloseDoor.h"
#include "EscapeStalingradZ/player/actions/ActionSearch.h"
#include "EscapeStalingradZ/player/actions/ActionExchangeEquipment.h"
#include "EscapeStalingradZ/player/actions/Command.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/player/PlayerActions.h"
#include "EscapeStalingradZ/player/PlayerC.h"
#include "Kismet/GameplayStatics.h"

UWOtherActions::UWOtherActions(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWOtherActions::NativeConstruct()
{
	Super::NativeConstruct();

	buttonFreeNewCharacter->OnClicked.AddDynamic(this, &UWOtherActions::OnClickFreeNewCharacter);
	buttonRotation->OnClicked.AddDynamic(this, &UWOtherActions::OnClickRotation);
	buttonOpenCloseDoor->OnClicked.AddDynamic(this, &UWOtherActions::OnClickOpenCloseDoor);
	buttonSearch->OnClicked.AddDynamic(this, &UWOtherActions::OnClickSearch);
	buttonExchangeEquipment->OnClicked.AddDynamic(this, &UWOtherActions::OnClickExchangeEquipment);
	goBack->OnClicked.AddDynamic(this, &UWOtherActions::GoBack);

	APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (player != nullptr) {
		controller = player;
		character = player->playerchara;
	}
}

void UWOtherActions::OnClickFreeNewCharacter()
{
	grid->deleteStatesFromTilesButSelected();
	command = NewObject<UActionFreeNewCharacter>(this);
	command->Execute(grid, character);
	controller->actions->command = NewObject<UActionFreeNewCharacter>(controller->actions);
	controller->actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWOtherActions::OnClickRotation()
{
	grid->deleteStatesFromTilesButSelected();
	command = NewObject<UActionMovementRotation>(this);
	command->Execute(grid, character);
	controller->actions->command = NewObject<UActionMovementRotation>(controller->actions);
	controller->actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWOtherActions::OnClickOpenCloseDoor()
{
	grid->deleteStatesFromTilesButSelected();
	command = NewObject<UActionOpenCloseDoor>(this);
	command->Execute(grid, character);
	controller->actions->command = NewObject<UActionOpenCloseDoor>(controller->actions);
	controller->actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWOtherActions::OnClickSearch()
{
	grid->deleteStatesFromTilesButSelected();
	command = NewObject<UActionSearch>(this);
	command->Execute(grid, character);
	controller->actions->command = NewObject<UActionSearch>(controller->actions);
	controller->actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWOtherActions::OnClickExchangeEquipment()
{
	grid->deleteStatesFromTilesButSelected();
	command = NewObject<UActionExchangeEquipment>(this);
	command->Execute(grid, character);
	controller->actions->command = NewObject<UActionExchangeEquipment>(controller->actions);
	controller->actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWOtherActions::GoBack()
{
	grid->deleteStatesFromTilesButSelected();
	controller->actions->command = nullptr;
	SetVisibility(ESlateVisibility::Hidden);
	actions->SetVisibleActionsAndOptions();
}

void UWOtherActions::SetButtonSearchEnabledOrDisabled()
{
	bool isEnabled = false;
	if (character != nullptr) {
		if (!character->attacked) {
			if (character->typeOfMovement == MovementType::Running && character->mp == 8) {
				isEnabled = true;
			}
			else if (character->typeOfMovement != MovementType::Running && character->mp == 4) {
				isEnabled = true;
			}
		}
	}
	if (isEnabled) {
		FIntPoint index = grid->GetTileIndexFromLocation(character->GetActorLocation());
		if (SearchTileInNeighbor(index)) {
			buttonSearch->SetIsEnabled(true);
		}
		else {
			buttonSearch->SetIsEnabled(false);
		}
	}
	else {
		buttonSearch->SetIsEnabled(false);
	}
}

void UWOtherActions::SetButtonFreeNewCharacterVisibilityAndEnabledOrDisabled()
{
	if (GetWorld()->GetName() == "AFriendWillBleed") {
		APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (player != nullptr && player->canExitTheRoom) {
			buttonFreeNewCharacter->SetIsEnabled(false);
		}
		else {
			buttonFreeNewCharacter->SetIsEnabled(true);
		}
	}
	else {
		buttonFreeNewCharacter->SetVisibility(ESlateVisibility::Hidden);
	}
	SetButtonExchangeEquipmentVisibilityAndEnabledOrDisabled();
}

void UWOtherActions::SetButtonExchangeEquipmentVisibilityAndEnabledOrDisabled()
{
	if (turn != nullptr) {
		if (turn->characters.Num() > 1 && character->mp>=2) {
			buttonExchangeEquipment->SetVisibility(ESlateVisibility::Visible);
			bool isEnabled = false;
			FIntPoint indice = grid->GetTileIndexFromLocation(character->GetActorLocation());
			FVector fv = character->GetActorForwardVector();
			FVector rv = character->GetActorRightVector();
			TArray<FIntPoint> indices = grid->GetFrontTiles(indice, fv, rv);
			for (FIntPoint l : indices) {
				APlayerCharacter* chara = Cast<APlayerCharacter>(grid->gridTiles[l].actor);
				if (chara != nullptr) {
					isEnabled = true;
					break;
				}
			}
			buttonExchangeEquipment->SetIsEnabled(isEnabled);
		}
		else {
			buttonExchangeEquipment->SetIsEnabled(false);
		}
	}
}

bool UWOtherActions::SearchTileInNeighbor(FIntPoint tile)
{
	if (grid->gridTiles[tile].types.Contains(TileType::Search)) {
		return true;
	}
	FVector fv = character->GetActorForwardVector();
	FVector rv = character->GetActorRightVector();
	TArray<FIntPoint> indices = grid->GetFrontTiles(tile, fv, rv);
	for (FIntPoint index : indices) {
		if (grid->gridTiles[index].types.Contains(TileType::Search)) {
			return true;
		}
	}
	return false;
}

