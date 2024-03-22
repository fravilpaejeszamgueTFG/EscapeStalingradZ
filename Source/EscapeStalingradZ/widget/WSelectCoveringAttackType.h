// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EscapeStalingradZ/enum/Enumerates.h"
#include "WSelectCoveringAttackType.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWSelectCoveringAttackType : public UUserWidget
{
	GENERATED_BODY()

public:

	UWSelectCoveringAttackType(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		class UButton* buttonHandToHand;
	UPROPERTY(meta = (BindWidget))
		class UButton* buttonNormalFire;
	UPROPERTY(meta = (BindWidget))
		class UButton* buttonSpreadFire;

	UPROPERTY(VisibleAnywhere)
		class APlayerCharacter* character;

	UPROPERTY(VisibleAnywhere)
		class ATurn* turn;

	UPROPERTY(VisibleAnywhere)
		class AUserHUD* hud;

	UPROPERTY(VisibleAnywhere, Category = Covering) TEnumAsByte<CoveringAttackType> typeOfAttack;

	UFUNCTION() void OnClickHandToHand();
	UFUNCTION() void OnClickNormalFire();
	UFUNCTION() void OnClickSpreadFire();
	UFUNCTION() void OnClickConfirm();

	UFUNCTION() void UpdateButtons();
};
