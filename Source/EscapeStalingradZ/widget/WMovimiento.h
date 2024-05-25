// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EscapeStalingradZ/enum/Enumerates.h"
#include "WMovimiento.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWMovimiento : public UUserWidget
{
	GENERATED_BODY()

public:

	UWMovimiento(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		class UBoton* buttonForward;
	UPROPERTY(meta = (BindWidget))
		class UBoton* buttonLateral;
	UPROPERTY(meta = (BindWidget))
		class UBoton* buttonBackward;
	UPROPERTY(meta = (BindWidget))
		class UBoton* buttonDiagonal;
	UPROPERTY(meta = (BindWidget))
		class UBoton* goBack;

	UPROPERTY(VisibleAnywhere) class ATurn* turn;

	UPROPERTY(VisibleAnywhere, Category = Grid) class AGrid* grid;
	UPROPERTY(VisibleAnywhere, Category = Character) class APlayerCharacter* character;
	UPROPERTY(VisibleAnywhere, Category = Character) class APlayerC* controller;
	UPROPERTY(VisibleAnywhere, Category = Command) class UCommand* command;
	UPROPERTY(VisibleAnywhere) class UWActions* actions;

	UFUNCTION() void OnClickForward();
	UFUNCTION() void OnClickLateral();
	UFUNCTION() void OnClickBackward();
	UFUNCTION() void OnClickDiagonal();
	UFUNCTION() void GoBack();
	UFUNCTION() void DisableButtonByMovementType(MovementType type);
	
};
