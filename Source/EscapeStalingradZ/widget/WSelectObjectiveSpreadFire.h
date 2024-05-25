// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WSelectObjectiveSpreadFire.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWSelectObjectiveSpreadFire : public UUserWidget
{
	GENERATED_BODY()

public:

	UWSelectObjectiveSpreadFire(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* NumberDiceLeftText;

	UFUNCTION() void SetNumberDiceLeft(int number);
	
};
