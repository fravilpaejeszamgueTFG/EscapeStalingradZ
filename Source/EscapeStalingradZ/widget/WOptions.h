// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WOptions.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWOptions : public UUserWidget
{
	GENERATED_BODY()

public:

	UWOptions(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		class UButton* Inventory;
	UPROPERTY(meta = (BindWidget))
		class UButton* Options;

	UPROPERTY(VisibleAnywhere)
		class AUserHUD* hud;


	UFUNCTION() void OnClickInventory();
	
};
