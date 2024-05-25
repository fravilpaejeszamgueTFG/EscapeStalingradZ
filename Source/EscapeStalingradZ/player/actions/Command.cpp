// Fill out your copyright notice in the Description page of Project Settings.


#include "Command.h"
#include "EscapeStalingradZ/Grid/Grid.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"


void UCommand::Execute(AGrid* grid, APlayerCharacter* character)
{

}

void UCommand::Action(AGrid* grid, FIntPoint tile, FIntPoint destinyTile)
{

}

bool UCommand::NeedLoF()
{
    return false;
}

