// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Command.h"
#include "ActionExchangeEquipment.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UActionExchangeEquipment : public UCommand
{
	GENERATED_BODY()

public:
	virtual void Execute(class AGrid* grid, class APlayerCharacter* character) override;

	virtual void Action(class AGrid* grid, FIntPoint tile, FIntPoint destinyTile) override;
};
