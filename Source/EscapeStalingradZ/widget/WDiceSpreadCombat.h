// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WDiceSpreadCombat.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWDiceSpreadCombat : public UUserWidget
{
	GENERATED_BODY()

public:

	UWDiceSpreadCombat(const FObjectInitializer& ObjectInitializer);

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

	UPROPERTY(VisibleAnywhere) int currentNumber;
	UPROPERTY(VisibleAnywhere) int numberToStun;
	UPROPERTY(VisibleAnywhere) int numberToKill;
	UPROPERTY(VisibleAnywhere) int numberDiceLeft;
	UPROPERTY(VisibleAnywhere) TArray<int> humanDicesLeft;
	UPROPERTY(VisibleAnywhere) bool attackInCovering = false;

	UPROPERTY(VisibleAnywhere) class AZombie* zombie;
	UPROPERTY(VisibleAnywhere) class AZombie* firstZombieIfCovering;
	UPROPERTY(VisibleAnywhere) FIntPoint tileZombie = FIntPoint(-1, -1);
	UPROPERTY(VisibleAnywhere) class APlayerCharacter* character;

	UPROPERTY(VisibleAnywhere)
		class AUserHUD* hud;

	UPROPERTY(EditDefaultsOnly, Category = "SpreadFire")
		TSubclassOf<class UWSelectObjectiveSpreadFire> SelectObjectiveWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpreadFire")
		class UWSelectObjectiveSpreadFire* SelectObjectiveWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Parameters")
		TSubclassOf<class AAnimatedTextAttack> textClass;

	FTimerHandle WaitTimer;

	UFUNCTION() void SetDices(TArray<int> humanDices, int targetDie);
	UFUNCTION() void NextDie(int targetDie);
	UFUNCTION() void AttackSpreadFire();
	UFUNCTION() void SelectObjetiveSpreadFire();
	UFUNCTION() void EndAttack();
	UFUNCTION() void RemoveDie();

	UFUNCTION(BlueprintImplementableEvent) void OnClickButtonRollAnimation();
	UFUNCTION(BlueprintImplementableEvent) void SetHumanDieImage(int number);
	UFUNCTION() void OnClickButtonConfirm();
	
};
