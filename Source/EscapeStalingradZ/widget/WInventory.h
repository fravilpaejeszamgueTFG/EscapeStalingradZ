// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WInventory.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWInventory : public UUserWidget
{
	GENERATED_BODY()

public:

	UWInventory(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AmmoNumber;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* FoodNumber;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MedKitNumber;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* SlotReadyWeapon;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* SlotSecondaryWeapon;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Slot1;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Slot2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Slot3;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Slot4;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Back;

	UPROPERTY(VisibleAnywhere)
		class AUserHUD* hud;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TEnumAsByte<enum EWeapon> selectedWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) bool hasSelectedWeapon = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character) class APlayerCharacter* character;

	UFUNCTION(BlueprintImplementableEvent) void SetImages();
	UFUNCTION(BlueprintImplementableEvent) void SetWeaponSlot1ImageByGivenWeapon(enum EWeapon weaponName);
	UFUNCTION(BlueprintImplementableEvent) void SetWeaponSlot2ImageByGivenWeapon(enum EWeapon weaponName);
	UFUNCTION(BlueprintImplementableEvent) void SetWeaponSlot3ImageByGivenWeapon(enum EWeapon weaponName);
	UFUNCTION(BlueprintImplementableEvent) void SetWeaponSlot4ImageByGivenWeapon(enum EWeapon weaponName);
	UFUNCTION(BlueprintImplementableEvent) void SetWeaponSlotReadyImageByGivenWeapon(enum EWeapon weaponName);
	UFUNCTION(BlueprintImplementableEvent) void SetWeaponSlotSecondaryImageByGivenWeapon(enum EWeapon weaponName);
	UFUNCTION() void OnClickSlot1();
	UFUNCTION() void OnClickSlot2();
	UFUNCTION() void OnClickSlot3();
	UFUNCTION() void OnClickSlot4();
	UFUNCTION() void OnClickSlotReadyWeapon();
	UFUNCTION() void OnClickSlotSecondaryWeapon();
	UFUNCTION() void OnClickGoBack();
	UFUNCTION() void SetTextNumbers();
	
};
