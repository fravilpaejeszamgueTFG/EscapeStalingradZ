// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WDicesCombat.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWDicesCombat : public UUserWidget
{
	GENERATED_BODY()

public:

	UWDicesCombat(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* DieNumber;
	UPROPERTY(meta = (BindWidget))
		class UImage* RollAnimation;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonRollAnimation;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonConfirm;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* NumberToStunText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* NumberToKillText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* NumberDiceLeftText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ButtonConfirmText;

	UPROPERTY(VisibleAnywhere) int currentNumber;
	UPROPERTY(VisibleAnywhere) int numberToStun;
	UPROPERTY(VisibleAnywhere) int numberToKill;
	UPROPERTY(VisibleAnywhere) int numberDiceLeft;
	UPROPERTY(EditAnywhere) FText confirmText;
	UPROPERTY(EditAnywhere) FText rollAgainText;
	UPROPERTY(VisibleAnywhere) TArray<int> humanDicesLeft;
	UPROPERTY(VisibleAnywhere) bool isAttackHandToHand;
	UPROPERTY(VisibleAnywhere) bool zombieStunned;
	UPROPERTY(VisibleAnywhere) bool attackInCovering = false;

	UPROPERTY(VisibleAnywhere) class AZombie* zombie;
	UPROPERTY(VisibleAnywhere) class APlayerCharacter* character;

	UPROPERTY(VisibleAnywhere)
		class AUserHUD* hud;

	FTimerHandle WaitTimer;

	UFUNCTION() void SetDices(TArray<int> humanDices, int targetDie, bool isHandToHand);
	UFUNCTION() void NextDie();
	UFUNCTION() void AttackInHandToHand();
	UFUNCTION() void AttackInFire();

	UFUNCTION(BlueprintImplementableEvent) void OnClickButtonRollAnimation();
	UFUNCTION(BlueprintImplementableEvent) void SetHumanDieImage(int number);
	UFUNCTION() void OnClickButtonConfirm();
	
};
