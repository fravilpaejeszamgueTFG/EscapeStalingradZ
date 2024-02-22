// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WCombat.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWCombat : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UWCombat(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UBoton* buttonNormalFire;
	UPROPERTY(meta = (BindWidget))
	class UBoton* buttonSpreadFire;
	UPROPERTY(meta = (BindWidget))
	class UBoton* buttonHandToHand;
	UPROPERTY(meta = (BindWidget))
	class UBoton* goBack;

	UPROPERTY(VisibleAnywhere) class ATurn* turn;

	UPROPERTY(VisibleAnywhere, Category = Grid) class AGrid* grid;
	UPROPERTY(VisibleAnywhere, Category = Character) class APlayerCharacter* character;
	UPROPERTY(VisibleAnywhere, Category = Character) class APlayerC* controller;
	UPROPERTY(VisibleAnywhere, Category = Command) class UCommand* command;
	UPROPERTY(VisibleAnywhere) class UWActions* actions;

	UFUNCTION() void OnClickNormalFire();
	UFUNCTION() void OnClickSpreadFire();
	UFUNCTION() void OnClickHandToHand();
	UFUNCTION() void GoBack();

	UFUNCTION() ESlateVisibility CanFireWeapon();
	UFUNCTION() ESlateVisibility CanFireWeaponInSpread();

};
