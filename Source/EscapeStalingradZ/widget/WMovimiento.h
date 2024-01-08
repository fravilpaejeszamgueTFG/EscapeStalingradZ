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
		class UMovementForward* buttonForward;
	UPROPERTY(meta = (BindWidget))
		class UMovementLateral* buttonLateral;
	UPROPERTY(meta = (BindWidget))
		class UMovementBackward* buttonBackward;
	UPROPERTY(meta = (BindWidget))
		class UAttack* buttonAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Grid) class AGrid* grid;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character) class APlayerCharacter* character;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character) class APlayerActions* actions;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Command) class UCommand* command;

	UFUNCTION() void OnClickForward();
	UFUNCTION() void OnClickAttack();
	UFUNCTION() void OnClickLateral();
	UFUNCTION() void OnClickBackward();
	
};
