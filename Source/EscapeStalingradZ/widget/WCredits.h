// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WCredits.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWCredits : public UUserWidget
{
	GENERATED_BODY()

public:

public:

	UWCredits(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* buttonMainMenu;

	UFUNCTION() void OnClickMainMenu();
	
};
