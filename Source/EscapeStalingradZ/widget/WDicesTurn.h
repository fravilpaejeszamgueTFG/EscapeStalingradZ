// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WDicesTurn.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWDicesTurn : public UUserWidget
{
	GENERATED_BODY()

public:

	UWDicesTurn(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* DieNumber;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* RollAnimation;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonRollAnimation;
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
		class UButton* ButtonConfirm;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* NumberToBeat;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ButtonConfirmText;

	UPROPERTY(VisibleAnywhere) class ATurn* turn;
	
	UPROPERTY(VisibleAnywhere)
		class UWSelectStartTurn* selectStartTurnWidget;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UWSelectStartTurn> selectStartTurnWidgetClass;
	

	UPROPERTY(VisibleAnywhere) int currentNumber;
	UPROPERTY(VisibleAnywhere) int targetNumber;
	UPROPERTY(EditAnywhere) FText confirmText;
	UPROPERTY(EditAnywhere) FText rollAgainText;

	UFUNCTION() void SetDices(int humanDie, int targetDie);

	UFUNCTION(BlueprintImplementableEvent) void OnClickButtonRollAnimation();
	UFUNCTION(BlueprintImplementableEvent) void SetHumanDieImage(int number);
	UFUNCTION() void OnClickButtonConfirm();
	UFUNCTION() void CreateOrSetSelectStartTurn();
	
};
