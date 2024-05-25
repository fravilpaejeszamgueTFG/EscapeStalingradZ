// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WReleaseLock.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWReleaseLock : public UUserWidget
{
	GENERATED_BODY()

public:

	UWReleaseLock(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* DieNumber;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* RollAnimation;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonRollAnimation;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonConfirm;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* NumberToBeat;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ButtonConfirmText;

	UPROPERTY(VisibleAnywhere) class ATurn* turn;
	UPROPERTY(VisibleAnywhere) class APlayerCharacter* character;

	UPROPERTY(VisibleAnywhere) int currentNumber;
	UPROPERTY(VisibleAnywhere) int targetNumber;

	UFUNCTION() void SetDicesNumbers();
	UFUNCTION() void SetDices(int humanDie, int targetDie);

	UFUNCTION(BlueprintImplementableEvent) void OnClickButtonRollAnimation();
	UFUNCTION(BlueprintImplementableEvent) void SetHumanDieImage(int number);
	UFUNCTION() void OnClickButtonConfirm();
	
};
