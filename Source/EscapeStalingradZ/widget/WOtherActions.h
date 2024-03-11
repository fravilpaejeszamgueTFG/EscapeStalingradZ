// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WOtherActions.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWOtherActions : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UWOtherActions(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UBoton* buttonRotation;
	UPROPERTY(meta = (BindWidget))
	class UBoton* buttonOpenCloseDoor;
	UPROPERTY(meta = (BindWidget))
	class UBoton* buttonSearch;
	UPROPERTY(meta = (BindWidget))
	class UBoton* goBack;

	UPROPERTY(VisibleAnywhere) class ATurn* turn;

	UPROPERTY(VisibleAnywhere, Category = Grid) class AGrid* grid;
	UPROPERTY(VisibleAnywhere, Category = Character) class APlayerCharacter* character;
	UPROPERTY(VisibleAnywhere, Category = Character) class APlayerC* controller;
	UPROPERTY(VisibleAnywhere, Category = Command) class UCommand* command;
	UPROPERTY(VisibleAnywhere) class UWActions* actions;

	UFUNCTION() void OnClickRotation();
	UFUNCTION() void OnClickOpenCloseDoor();
	UFUNCTION() void OnClickSearch();
	UFUNCTION() void GoBack();
	UFUNCTION() void SetButtonSearchEnabledOrDisabled();
	UFUNCTION() bool SearchTileInNeighbor(FIntPoint tile);
};
