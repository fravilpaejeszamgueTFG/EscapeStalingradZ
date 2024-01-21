// Fill out your copyright notice in the Description page of Project Settings.


#include "WMovimiento.h"
#include "WActions.h"
#include "buttons/Boton.h"
#include "EscapeStalingradZ/Grid/Grid.h"
#include "EscapeStalingradZ/player/actions/Command.h"
#include "EscapeStalingradZ/player/actions/ActionMovementForward.h"
#include "EscapeStalingradZ/player/actions/ActionMovementLateral.h"
#include "EscapeStalingradZ/player/actions/ActionMovementBackward.h"
#include "EscapeStalingradZ/player/actions/ActionDiagonalMovement.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/player/PlayerActions.h"
#include "EscapeStalingradZ/player/PlayerC.h"
#include "Kismet/GameplayStatics.h"


UWMovimiento::UWMovimiento(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UWMovimiento::NativeConstruct()
{
	Super::NativeConstruct();

	buttonForward->OnClicked.AddDynamic(this, &UWMovimiento::OnClickForward);
	buttonLateral->OnClicked.AddDynamic(this, &UWMovimiento::OnClickLateral);
	buttonBackward->OnClicked.AddDynamic(this, &UWMovimiento::OnClickBackward);
	buttonDiagonal->OnClicked.AddDynamic(this, &UWMovimiento::OnClickDiagonal);
	goBack->OnClicked.AddDynamic(this, &UWMovimiento::GoBack);

	APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (player != nullptr) {
		controller = player;
		character = player->playerchara;
	}
}

void UWMovimiento::OnClickForward()
{
	grid->deleteStatesFromTilesButSelected();
	command = NewObject<UActionMovementForward>(this);
    command->Execute(grid, character);
	controller->actions->command = NewObject<UActionMovementForward>(controller->actions);
	controller->actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWMovimiento::OnClickLateral()
{
	grid->deleteStatesFromTilesButSelected();
	command = NewObject<UActionMovementLateral>(this);
	command->Execute(grid, character);
	controller->actions->command = NewObject<UActionMovementLateral>(controller->actions);
	controller->actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWMovimiento::OnClickBackward()
{
	grid->deleteStatesFromTilesButSelected();
	command = NewObject<UActionMovementBackward>(this);
	command->Execute(grid, character);
	controller->actions->command = NewObject<UActionMovementBackward>(controller->actions);
	controller->actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWMovimiento::OnClickDiagonal()
{
	grid->deleteStatesFromTilesButSelected();
	command = NewObject<UActionDiagonalMovement>(this);
	command->Execute(grid, character);
	controller->actions->command = NewObject<UActionDiagonalMovement>(controller->actions);
	controller->actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWMovimiento::GoBack()
{
	grid->deleteStatesFromTilesButSelected();
	controller->actions->command = nullptr;
	SetVisibility(ESlateVisibility::Hidden);
	actions->SetVisibility(ESlateVisibility::Visible);

}

void UWMovimiento::DisableButtonByMovementType(MovementType type)
{
	if (type == MovementType::Running) {
		buttonLateral->SetVisibility(ESlateVisibility::Hidden);
		buttonBackward->SetVisibility(ESlateVisibility::Hidden);
		buttonDiagonal->SetVisibility(ESlateVisibility::Visible);
		buttonForward->SetVisibility(ESlateVisibility::Visible);
	}
	else if (type == MovementType::Stationary) {
		buttonLateral->SetVisibility(ESlateVisibility::Hidden);
		buttonBackward->SetVisibility(ESlateVisibility::Hidden);
		buttonDiagonal->SetVisibility(ESlateVisibility::Hidden);
		buttonForward->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		buttonLateral->SetVisibility(ESlateVisibility::Visible);
		buttonBackward->SetVisibility(ESlateVisibility::Visible);
		buttonDiagonal->SetVisibility(ESlateVisibility::Visible);
		buttonForward->SetVisibility(ESlateVisibility::Visible);
	}
}

