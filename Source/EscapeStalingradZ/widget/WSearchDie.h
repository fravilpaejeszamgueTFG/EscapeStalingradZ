// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WSearchDie.generated.h"


UENUM()
enum ObjectName
{
	Ammo     UMETA(DisplayName = "Ammo"),
	Food     UMETA(DisplayName = "Food"),
	MedKit	   UMETA(DisplayName = "MedKit"),
	WKnife     UMETA(DisplayName = "WKnife"),
	WLuger	   UMETA(DisplayName = "WLuger"),
	SecondFavWeapon	   UMETA(DisplayName = "SecondFavWeapon"),
	FavWeapon	   UMETA(DisplayName = "FavWeapon"),
};

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

	//nombre del mapa
	UPROPERTY(BlueprintReadOnly, Category = gridName) TEnumAsByte<enum ScenarioName> gridName;
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
	UFUNCTION() void SetWeaponInFreeSlot(enum EWeapon weaponName);

	
};
