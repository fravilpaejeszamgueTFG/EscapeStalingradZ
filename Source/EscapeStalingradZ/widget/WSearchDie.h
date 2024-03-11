// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WSearchDie.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWSearchDie : public UUserWidget
{
	GENERATED_BODY()

public:

	UWSearchDie(const FObjectInitializer& ObjectInitializer);

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
		class UTextBlock* ButtonConfirmText;

	UPROPERTY(VisibleAnywhere) class APlayerCharacter* character;
	UPROPERTY(VisibleAnywhere) class AUserHUD* hud;

	UPROPERTY(VisibleAnywhere) int currentNumber;

	UFUNCTION() void SetDie(int numberOfDie);

	UFUNCTION(BlueprintImplementableEvent) void OnClickButtonRollAnimation();
	UFUNCTION(BlueprintImplementableEvent) void SetHumanDieImage(int number);
	UFUNCTION() void OnClickButtonConfirm();

	
};
