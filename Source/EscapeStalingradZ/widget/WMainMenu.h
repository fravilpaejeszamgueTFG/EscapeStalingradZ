// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UWMainMenu(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonNewGame;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonLoadGame;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonSettings;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonRules;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonExit;

	UPROPERTY(EditDefaultsOnly, Category = "NewOrLoadGame")
		TSubclassOf<class UWNewOrLoadGame> NewOrLoadGameWidgetClass;

	UFUNCTION() void OnClickButtonNewGame();
	UFUNCTION() void OnClickButtonLoadGame();
	UFUNCTION() void OnClickButtonSettings();
	UFUNCTION() void OnClickButtonRules();
	UFUNCTION() void OnClickButtonExit();
	UFUNCTION() void EnableLoadButton();

};
