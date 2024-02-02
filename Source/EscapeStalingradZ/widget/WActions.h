// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WActions.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWActions : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UWActions(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UBoton* buttonMovement;
	UPROPERTY(meta = (BindWidget))
	class UBoton* buttonCombat;
	UPROPERTY(meta = (BindWidget))
	class UBoton* buttonActions;
	UPROPERTY(meta = (BindWidget))
	class UBoton* endTurn;

	UPROPERTY(VisibleAnywhere)
	class UWMovimiento* movementWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWMovimiento> movementWidgetClass;

	UPROPERTY(VisibleAnywhere)
	class UWCombat* combatWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWCombat> combatWidgetClass;

	UPROPERTY(VisibleAnywhere)
	class UWOtherActions* actionWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWOtherActions> actionWidgetClass;

	UPROPERTY(VisibleAnywhere)
	class APlayerCharacter* character;

	UPROPERTY(VisibleAnywhere)
	class ATurn* turn;

	UPROPERTY(VisibleAnywhere)
	class APlayerC* controller;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class AGrid* grid;

	UPROPERTY(VisibleAnywhere)
	class AUserHUD* hud;

	UFUNCTION() void OnClickMovement();
	UFUNCTION() void OnClickCombat();
	UFUNCTION() void OnClickActions();
	UFUNCTION() void EndTurn();
	UFUNCTION() void HideWidgets();
	UFUNCTION() void DisableButtonByMovementType(enum MovementType type);

};
