// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
		class UBoton* buttonAttack;
	UPROPERTY(meta = (BindWidget))
		class UBoton* buttonNormalFire;
	UPROPERTY(meta = (BindWidget))
		class UBoton* buttonHandToHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Grid) class AGrid* grid;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character) class APlayerCharacter* character;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character) class APlayerActions* actions;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Command) class UCommand* command;

	UFUNCTION() void OnClickForward();
	UFUNCTION() void OnClickAttack();
	UFUNCTION() void OnClickLateral();
	UFUNCTION() void OnClickBackward();
	UFUNCTION() void OnClickDiagonal();
	UFUNCTION() void OnClickNormalFire();
	UFUNCTION() void OnClickHandToHand();
	
};
