// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Command.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class ESCAPESTALINGRADZ_API UCommand : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable) virtual void Execute(class AGrid* grid, class APlayerCharacter* character);
	UFUNCTION(BlueprintCallable) virtual void Action(class AGrid* grid, FIntPoint tile, FIntPoint destinyTile);
	UFUNCTION(BlueprintCallable) virtual bool NeedLoF();
};
