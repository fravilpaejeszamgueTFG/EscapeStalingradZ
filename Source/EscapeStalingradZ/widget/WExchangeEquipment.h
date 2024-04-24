// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EscapeStalingradZ/enum/Enumerates.h"
#include "WExchangeEquipment.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWExchangeEquipment : public UUserWidget
{
	GENERATED_BODY()

public:

	UWExchangeEquipment(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	//personaje 1
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AmmoNumber;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* FoodNumber;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MedKitNumber;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* SlotAmmo1;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* SlotFood1;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* SlotMedKit1;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Slot1;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Slot2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Slot3;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Slot4;

	//personaje 2
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AmmoNumber2;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* FoodNumber2;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MedKitNumber2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* SlotAmmo2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* SlotFood2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* SlotMedKit2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Slot1_2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Slot2_2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Slot3_2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Slot4_2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Back;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TEnumAsByte<EWeapon> selectedWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) bool hasSelectedCharacter1 = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) bool hasSelectedCharacter2 = false;

	UPROPERTY(VisibleAnywhere) int selectedSlot1 = 0; // de 1 a 4 para las armas
	UPROPERTY(VisibleAnywhere) int selectedSlot2 = 0; // de 1 a 4 para las armas
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character) class APlayerCharacter* character1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character) class APlayerCharacter* character2;

	UPROPERTY(VisibleAnywhere) class UWBetweenScenarios* betweenWidget;

	UFUNCTION(BlueprintImplementableEvent) void SetImages();
	UFUNCTION(BlueprintImplementableEvent) void SetWeaponImageInSlot(EWeapon weaponName, class UButton* SlotNumber);
	UFUNCTION(BlueprintImplementableEvent) void ResetSlotsColors();
	UFUNCTION(BlueprintImplementableEvent) void SetSlotSelected(class UButton* SlotNumber);
	UFUNCTION() void OnClickSlot1();
	UFUNCTION() void OnClickSlot2();
	UFUNCTION() void OnClickSlot3();
	UFUNCTION() void OnClickSlot4();
	UFUNCTION() void OnClickSlot1_2();
	UFUNCTION() void OnClickSlot2_2();
	UFUNCTION() void OnClickSlot3_2();
	UFUNCTION() void OnClickSlot4_2();
	UFUNCTION() void OnClickSlotFood1();
	UFUNCTION() void OnClickSlotAmmo1();
	UFUNCTION() void OnClickSlotMedKit1();
	UFUNCTION() void OnClickSlotFood2();
	UFUNCTION() void OnClickSlotAmmo2();
	UFUNCTION() void OnClickSlotMedKit2();
	UFUNCTION() void OnClickGoBack();
	UFUNCTION() void SetTextNumbers();
	UFUNCTION() void ResetSelectedCharacter();
	UFUNCTION() void SetSelectedWeaponGivenSlotAndWeaponCharacter1(class UButton* SlotNumber, EWeapon weaponName);
	UFUNCTION() void SetSelectedWeaponGivenSlotAndWeaponCharacter2(class UButton* SlotNumber, EWeapon weaponName);
	UFUNCTION() void ChangeWeaponBetweenCharacters(class APlayerCharacter* chara1, class APlayerCharacter* chara2, EWeapon weaponName);
	UFUNCTION() void DisableButtons();
	UFUNCTION() void DisableItemsButtons1();
	UFUNCTION() void DisableItemsButtons2();
	UFUNCTION() void EnableWeaponsCharacter1(bool enableWeapon);
	UFUNCTION() void EnableWeaponsCharacter2(bool enableWeapon);
	UFUNCTION() void DisableButtonsGivenSlotCharacter1(class UButton* SlotNumber);
	UFUNCTION() void DisableButtonsGivenSlotCharacter2(class UButton* SlotNumber);
	UFUNCTION() void DisableButtonGivenSlotAndWeapon(class UButton* SlotNumber, EWeapon weaponName);
	UFUNCTION() void DisableButtonGivenSlotAndItemNumber(class UButton* SlotNumber, int numberCharacter1, int numberCharacter2);
	UFUNCTION() void DisableButtonsOnClickItem(bool enableFood, bool enableAmmo, bool enableMedKit);
	UFUNCTION() void SetWeaponInGivenSlotByBool(class UButton* SlotNumber1, class UButton* SlotNumber2, bool in1, EWeapon weaponName);
	
};
