// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Command.h"
#include "ActionMovementRotation.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UActionMovementRotation : public UCommand
{
	GENERATED_BODY()

public:
	virtual void Execute(class AGrid* grid, class APlayerCharacter* character) override;

	virtual void Action(class AGrid* grid, FIntPoint tile, FIntPoint destinyTile) override;

	void CheckMP(class APlayerCharacter* character, FVector oldFV, FVector newFV);

	FRotator GetRotationDirection(FIntPoint tile, FIntPoint destinyTile);
	
};
