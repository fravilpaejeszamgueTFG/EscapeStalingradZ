// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EscapeStalingradZ/enum/Enumerates.h"
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

	UPROPERTY(VisibleAnywhere) int numberOfSearch = 0;

	UPROPERTY(VisibleAnywhere) FIntPoint searchTileIndex = FIntPoint(-1,-1);

	UPROPERTY(VisibleAnywhere) bool unhidePlayerInfo = true;

	//nombre del mapa
	UPROPERTY(BlueprintReadOnly, Category = gridName) TEnumAsByte<ScenarioName> gridName;
	//nombre del mapa
	UPROPERTY(BlueprintReadOnly, Category = gridName) TMap<TEnumAsByte<ObjectName>, int> objectsWon; //nombre objeto + cantidad

	UFUNCTION() void SetDie(int numberOfDie);

	UFUNCTION(BlueprintImplementableEvent) void OnClickButtonRollAnimation();
	UFUNCTION(BlueprintImplementableEvent) void SetHumanDieImage(int number);
	UFUNCTION() void OnClickButtonConfirm();
	UFUNCTION() void SetSearchingObjects();
	UFUNCTION() void SetSearchingObjectsFubar();
	UFUNCTION() void SetSearchingObjectsAFriendWillBleed();
	UFUNCTION() void SetSearchingObjectsWakeUpTimeToDie();
	UFUNCTION() void SetSearchingObjectsStash();
	UFUNCTION() void SetSearchingObjectsMoveAlong();
	UFUNCTION() void SetObjectWonToCharacter(ObjectName name, int number);
	UFUNCTION() void SetWeaponInFreeSlot(EWeapon weaponName);
	UFUNCTION() int GetNumberOfWidgetFromScenarioName();
	UFUNCTION(BlueprintImplementableEvent) void SetActiveSearchObjectsWidget(int number);


	
};
