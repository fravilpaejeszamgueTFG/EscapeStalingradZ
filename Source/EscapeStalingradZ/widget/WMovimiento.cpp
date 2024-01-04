// Fill out your copyright notice in the Description page of Project Settings.


#include "WMovimiento.h"
#include "buttons/MovementForward.h"
#include "EscapeStalingradZ/Grid/Grid.h"
#include "EscapeStalingradZ/player/actions/Command.h"
#include "EscapeStalingradZ/player/actions/ActionMovementForward.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/player/PlayerActions.h"


UWMovimiento::UWMovimiento(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UWMovimiento::NativeConstruct()
{
	Super::NativeConstruct();

	buttonForward->OnClicked.AddDynamic(this, &UWMovimiento::OnClickForward);
}

void UWMovimiento::OnClickForward()
{
	command = NewObject<UActionMovementForward>(this);
    command->Execute(grid, character);
	actions->command = NewObject<UActionMovementForward>(actions);
	actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}
