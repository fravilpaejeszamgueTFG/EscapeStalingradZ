// Fill out your copyright notice in the Description page of Project Settings.


#include "WMovimiento.h"
#include "buttons/Boton.h"
#include "EscapeStalingradZ/Grid/Grid.h"
#include "EscapeStalingradZ/player/actions/Command.h"
#include "EscapeStalingradZ/player/actions/ActionMovementForward.h"
#include "EscapeStalingradZ/player/actions/ActionMovementLateral.h"
#include "EscapeStalingradZ/player/actions/ActionMovementBackward.h"
#include "EscapeStalingradZ/player/actions/ActionMovementRotation.h"
#include "EscapeStalingradZ/player/actions/ActionDiagonalMovement.h"
#include "EscapeStalingradZ/player/actions/ActionNormalFire.h"
#include "EscapeStalingradZ/player/actions/ActionHandToHand.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/player/PlayerActions.h"
#include "EscapeStalingradZ/player/PlayerC.h"
#include "EscapeStalingradZ/turn/Turn.h"
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
	buttonRotation->OnClicked.AddDynamic(this, &UWMovimiento::OnClickRotation);
	buttonAttack->OnClicked.AddDynamic(this, &UWMovimiento::OnClickAttack);
	buttonNormalFire->OnClicked.AddDynamic(this, &UWMovimiento::OnClickNormalFire);
	buttonHandToHand->OnClicked.AddDynamic(this, &UWMovimiento::OnClickHandToHand);
	endTurn->OnClicked.AddDynamic(this, &UWMovimiento::EndTurn);

	APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (player != nullptr) {
		controller = player;
	}
}

void UWMovimiento::OnClickForward()
{
	command = NewObject<UActionMovementForward>(this);
    command->Execute(grid, character);
	controller->actions->command = NewObject<UActionMovementForward>(controller->actions);
	controller->actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWMovimiento::OnClickAttack()
{
	character->getArcOfFire();
}

void UWMovimiento::OnClickLateral()
{
	command = NewObject<UActionMovementLateral>(this);
	command->Execute(grid, character);
	controller->actions->command = NewObject<UActionMovementLateral>(controller->actions);
	controller->actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWMovimiento::OnClickBackward()
{
	command = NewObject<UActionMovementBackward>(this);
	command->Execute(grid, character);
	controller->actions->command = NewObject<UActionMovementBackward>(controller->actions);
	controller->actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWMovimiento::OnClickDiagonal()
{
	command = NewObject<UActionDiagonalMovement>(this);
	command->Execute(grid, character);
	controller->actions->command = NewObject<UActionDiagonalMovement>(controller->actions);
	controller->actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWMovimiento::OnClickRotation()
{
	command = NewObject<UActionMovementRotation>(this);
	command->Execute(grid, character);
	controller->actions->command = NewObject<UActionMovementRotation>(controller->actions);
	controller->actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWMovimiento::OnClickNormalFire()
{
	command = NewObject<UActionNormalFire>(this);
	command->Execute(grid, character);
	controller->actions->command = NewObject<UActionNormalFire>(controller->actions);
	controller->actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}
void UWMovimiento::OnClickHandToHand()
{
	command = NewObject<UActionHandToHand>(this);
	command->Execute(grid, character);
	controller->actions->command = NewObject<UActionHandToHand>(controller->actions);
	controller->actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWMovimiento::EndTurn()
{
	if (turn != nullptr) {
		grid->deleteStatesFromTiles();
		turn->nextCharacter();
		controller->actions->command = nullptr;
	}
	SetVisibility(ESlateVisibility::Hidden);
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

