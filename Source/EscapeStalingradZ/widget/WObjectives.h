// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WObjectives.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWObjectives : public UUserWidget
{
	GENERATED_BODY()

public:

	UWObjectives(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	
};
