// Fill out your copyright notice in the Description page of Project Settings.


#include "WMovimiento.h"
#include "buttons/Boton.h"
#include "EscapeStalingradZ/Grid/Grid.h"
#include "EscapeStalingradZ/player/actions/Command.h"
#include "EscapeStalingradZ/player/actions/ActionMovementForward.h"
#include "EscapeStalingradZ/player/actions/ActionMovementLateral.h"
#include "EscapeStalingradZ/player/actions/ActionMovementBackward.h"
#include "EscapeStalingradZ/player/actions/ActionNormalFire.h"
#include "EscapeStalingradZ/player/actions/ActionHandToHand.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/player/PlayerActions.h"


UWMovimiento::UWMovimiento(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UWMovimiento::NativeConstruct()
{
	Super::NativeConstruct();

	buttonForward->OnClicked.AddDynamic(this, &UWMovimiento::OnClickForward);
	buttonLateral->OnClicked.AddDynamic(this, &UWMovimiento::OnClickLateral);
	buttonBackward->OnClicked.AddDynamic(this, &UWMovimiento::OnClickBackward);
	buttonAttack->OnClicked.AddDynamic(this, &UWMovimiento::OnClickAttack);
	buttonNormalFire->OnClicked.AddDynamic(this, &UWMovimiento::OnClickNormalFire);
	buttonHandToHand->OnClicked.AddDynamic(this, &UWMovimiento::OnClickHandToHand);
}

void UWMovimiento::OnClickForward()
{
	command = NewObject<UActionMovementForward>(this);
    command->Execute(grid, character);
	actions->command = NewObject<UActionMovementForward>(actions);
	actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWMovimiento::OnClickAttack()
{
	character->getArcOfFire();
}

void UWMovimiento::OnClickLateral()
{
	command = NewObject<UActionMovementLateral>(this);
	command->Execute(grid, character);
	actions->command = NewObject<UActionMovementLateral>(actions);
	actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWMovimiento::OnClickBackward()
{
	command = NewObject<UActionMovementBackward>(this);
	command->Execute(grid, character);
	actions->command = NewObject<UActionMovementBackward>(actions);
	actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWMovimiento::OnClickNormalFire()
{
	command = NewObject<UActionNormalFire>(this);
	command->Execute(grid, character);
	actions->command = NewObject<UActionNormalFire>(actions);
	actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}
void UWMovimiento::OnClickHandToHand()
{
	command = NewObject<UActionHandToHand>(this);
	command->Execute(grid, character);
	actions->command = NewObject<UActionHandToHand>(actions);
	actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

