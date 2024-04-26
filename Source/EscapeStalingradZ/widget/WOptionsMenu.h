// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WOptionsMenu.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWOptionsMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UWOptionsMenu(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		class UButton* Resume;
	UPROPERTY(meta = (BindWidget))
		class UButton* RestartLevel;
	UPROPERTY(meta = (BindWidget))
		class UButton* Settings;
	UPROPERTY(meta = (BindWidget))
		class UButton* MainMenu;

	UFUNCTION() void OnClickResume();
	UFUNCTION() void OnClickRestartLevel();
	UFUNCTION() void OnClickSettings();
	UFUNCTION() void OnClickMainMenu();
	
};
