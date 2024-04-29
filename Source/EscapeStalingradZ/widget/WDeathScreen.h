// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WDeathScreen.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWDeathScreen : public UUserWidget
{
	GENERATED_BODY()

public:

	UWDeathScreen(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* buttonRestart;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* buttonMainMenu;

	UFUNCTION(BlueprintImplementableEvent) void StartAnimation();
	UFUNCTION() void OnClickRestart();
	UFUNCTION() void OnClickMainMenu();

	
};
