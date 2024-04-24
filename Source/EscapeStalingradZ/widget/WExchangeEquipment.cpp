// Fill out your copyright notice in the Description page of Project Settings.


#include "WExchangeEquipment.h"
#include "Components/Button.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/weapon/Weapon.h"
#include "Components/TextBlock.h"
#include "WBetweenScenarios.h"

UWExchangeEquipment::UWExchangeEquipment(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWExchangeEquipment::NativeConstruct()
{
	Super::NativeConstruct();

	Back->OnClicked.AddDynamic(this, &UWExchangeEquipment::OnClickGoBack);
	Slot1->OnClicked.AddDynamic(this, &UWExchangeEquipment::OnClickSlot1);
	Slot2->OnClicked.AddDynamic(this, &UWExchangeEquipment::OnClickSlot2);
	Slot3->OnClicked.AddDynamic(this, &UWExchangeEquipment::OnClickSlot3);
	Slot4->OnClicked.AddDynamic(this, &UWExchangeEquipment::OnClickSlot4);
	Slot1_2->OnClicked.AddDynamic(this, &UWExchangeEquipment::OnClickSlot1_2);
	Slot2_2->OnClicked.AddDynamic(this, &UWExchangeEquipment::OnClickSlot2_2);
	Slot3_2->OnClicked.AddDynamic(this, &UWExchangeEquipment::OnClickSlot3_2);
	Slot4_2->OnClicked.AddDynamic(this, &UWExchangeEquipment::OnClickSlot4_2);
	SlotFood1->OnClicked.AddDynamic(this, &UWExchangeEquipment::OnClickSlotFood1);
	SlotFood2->OnClicked.AddDynamic(this, &UWExchangeEquipment::OnClickSlotFood2);
	SlotAmmo1->OnClicked.AddDynamic(this, &UWExchangeEquipment::OnClickSlotAmmo1);
	SlotAmmo2->OnClicked.AddDynamic(this, &UWExchangeEquipment::OnClickSlotAmmo2);
	SlotMedKit1->OnClicked.AddDynamic(this, &UWExchangeEquipment::OnClickSlotMedKit1);
	SlotMedKit2->OnClicked.AddDynamic(this, &UWExchangeEquipment::OnClickSlotMedKit2);

}

void UWExchangeEquipment::OnClickSlot1()
{
	if (character1 != nullptr) {
		if (hasSelectedCharacter2) {
			selectedSlot2 = 1;
		}
		else if (!hasSelectedCharacter1) {
			selectedSlot1 = 1;
		}
		SetSelectedWeaponGivenSlotAndWeaponCharacter1(Slot1, character1->weapon1);
	}
}

void UWExchangeEquipment::OnClickSlot2()
{
	if (character1 != nullptr) {
		if (hasSelectedCharacter2) {
			selectedSlot2 = 2;
		}
		else if (!hasSelectedCharacter1) {
			selectedSlot1 = 2;
		}
		SetSelectedWeaponGivenSlotAndWeaponCharacter1(Slot2, character1->weapon2);
	}
}

void UWExchangeEquipment::OnClickSlot3()
{
	if (character1 != nullptr) {
		if (hasSelectedCharacter2) {
			selectedSlot2 = 3;
		}
		else if (!hasSelectedCharacter1) {
			selectedSlot1 = 3;
		}
		SetSelectedWeaponGivenSlotAndWeaponCharacter1(Slot3, character1->weapon3);
	}
}

void UWExchangeEquipment::OnClickSlot4()
{
	if (character1 != nullptr) {
		if (hasSelectedCharacter2) {
			selectedSlot2 = 4;
		}
		else if (!hasSelectedCharacter1) {
			selectedSlot1 = 4;
		}
		SetSelectedWeaponGivenSlotAndWeaponCharacter1(Slot4, character1->weapon4);
	}
}

void UWExchangeEquipment::OnClickSlot1_2()
{
	if (character2 != nullptr) {
		if (hasSelectedCharacter1) {
			selectedSlot2 = 1;
		}
		else if (!hasSelectedCharacter2) {
			selectedSlot1 = 1;
		}
		SetSelectedWeaponGivenSlotAndWeaponCharacter2(Slot1_2, character2->weapon1);
	}
}

void UWExchangeEquipment::OnClickSlot2_2()
{
	if (character2 != nullptr) {
		if (hasSelectedCharacter1) {
			selectedSlot2 = 2;
		}
		else if (!hasSelectedCharacter2) {
			selectedSlot1 = 2;
		}
		SetSelectedWeaponGivenSlotAndWeaponCharacter2(Slot2_2, character2->weapon2);
	}
}

void UWExchangeEquipment::OnClickSlot3_2()
{
	if (character2 != nullptr) {
		if (hasSelectedCharacter1) {
			selectedSlot2 = 3;
		}
		else if (!hasSelectedCharacter2) {
			selectedSlot1 = 3;
		}
		SetSelectedWeaponGivenSlotAndWeaponCharacter2(Slot3_2, character2->weapon3);
	}
}

void UWExchangeEquipment::OnClickSlot4_2()
{
	if (character2 != nullptr) {
		if (hasSelectedCharacter1) {
			selectedSlot2 = 4;
		}
		else if (!hasSelectedCharacter2) {
			selectedSlot1 = 4;
		}
		SetSelectedWeaponGivenSlotAndWeaponCharacter2(Slot4_2, character2->weapon4);
	}
}

void UWExchangeEquipment::OnClickSlotFood1()
{
	if (character1 != nullptr) {
		if (hasSelectedCharacter1) {
			ResetSelectedCharacter();
		}
		else {
			if (hasSelectedCharacter2) {
				character1->food++;
				character2->food--;
				SetTextNumbers();
				ResetSelectedCharacter();
			}
			else {
				hasSelectedCharacter1 = true;
				DisableButtonsOnClickItem(true, false, false);
				ResetSlotsColors();
				SetSlotSelected(SlotFood1);
			}
		}
	}
}

void UWExchangeEquipment::OnClickSlotFood2()
{
	if (character2 != nullptr) {
		if (hasSelectedCharacter2) {
			ResetSelectedCharacter();
		}
		else {
			if (hasSelectedCharacter1) {
				character2->food++;
				character1->food--;
				SetTextNumbers();
				ResetSelectedCharacter();
			}
			else {
				hasSelectedCharacter2 = true;
				DisableButtonsOnClickItem(true, false, false);
				ResetSlotsColors();
				SetSlotSelected(SlotFood2);
			}
		}
	}
}

void UWExchangeEquipment::OnClickSlotAmmo1()
{
	if (character1 != nullptr) {
		if (hasSelectedCharacter1) {
			ResetSelectedCharacter();
		}
		else {
			if (hasSelectedCharacter2) {
				character1->ammo++;
				character2->ammo--;
				SetTextNumbers();
				ResetSelectedCharacter();
			}
			else {
				hasSelectedCharacter1 = true;
				DisableButtonsOnClickItem(false, true, false);
				ResetSlotsColors();
				SetSlotSelected(SlotAmmo1);
			}
		}
	}
}

void UWExchangeEquipment::OnClickSlotAmmo2()
{
	if (character2 != nullptr) {
		if (hasSelectedCharacter2) {
			ResetSelectedCharacter();
		}
		else {
			if (hasSelectedCharacter1) {
				character2->ammo++;
				character1->ammo--;
				SetTextNumbers();
				ResetSelectedCharacter();
			}
			else {
				hasSelectedCharacter2 = true;
				DisableButtonsOnClickItem(false, true, false);
				ResetSlotsColors();
				SetSlotSelected(SlotAmmo2);
			}
		}
	}
}

void UWExchangeEquipment::OnClickSlotMedKit1()
{
	if (character1 != nullptr) {
		if (hasSelectedCharacter1) {
			ResetSelectedCharacter();
		}
		else {
			if (hasSelectedCharacter2) {
				character1->medkit++;
				character2->medkit--;
				SetTextNumbers();
				ResetSelectedCharacter();
			}
			else {
				hasSelectedCharacter1 = true;
				DisableButtonsOnClickItem(false, false, true);
				ResetSlotsColors();
				SetSlotSelected(SlotMedKit1);
			}
		}
	}
}

void UWExchangeEquipment::OnClickSlotMedKit2()
{
	if (character2 != nullptr) {
		if (hasSelectedCharacter2) {
			ResetSelectedCharacter();
		}
		else {
			if (hasSelectedCharacter1) {
				character2->medkit++;
				character1->medkit--;
				SetTextNumbers();
				ResetSelectedCharacter();
			}
			else {
				hasSelectedCharacter2 = true;
				DisableButtonsOnClickItem(false, false, true);
				ResetSlotsColors();
				SetSlotSelected(SlotMedKit2);
			}
		}
	}
}

void UWExchangeEquipment::OnClickGoBack()
{
	character1->ChangePrimaryAndSecondaryWeaponAfterExchange();
	character2->ChangePrimaryAndSecondaryWeaponAfterExchange();
	if (betweenWidget != nullptr) {
		betweenWidget->SetFoodText();
		betweenWidget->SetMedKitText();
		betweenWidget->SetAmmoText();
	}
	SetVisibility(ESlateVisibility::Hidden);
}

void UWExchangeEquipment::SetTextNumbers()
{
	if (character1 != nullptr) {
		AmmoNumber->SetText(FText::FromString(FString("x").Append(FString::FromInt(character1->ammo))));
		FoodNumber->SetText(FText::FromString(FString("x").Append(FString::FromInt(character1->food))));
		MedKitNumber->SetText(FText::FromString(FString("x").Append(FString::FromInt(character1->medkit))));
	}
	if (character2 != nullptr) {
		AmmoNumber2->SetText(FText::FromString(FString("x").Append(FString::FromInt(character2->ammo))));
		FoodNumber2->SetText(FText::FromString(FString("x").Append(FString::FromInt(character2->food))));
		MedKitNumber2->SetText(FText::FromString(FString("x").Append(FString::FromInt(character2->medkit))));
	}
}

void UWExchangeEquipment::ResetSelectedCharacter()
{
	hasSelectedCharacter1 = false;
	hasSelectedCharacter2 = false;
	ResetSlotsColors();
	DisableButtons();
}

void UWExchangeEquipment::SetSelectedWeaponGivenSlotAndWeaponCharacter1(UButton* SlotNumber, EWeapon weaponName)
{
	if (hasSelectedCharacter1) {
		selectedWeapon = EWeapon::None;
		ResetSelectedCharacter();
	}
	else {
		if (hasSelectedCharacter2) {
			ChangeWeaponBetweenCharacters(character1, character2, weaponName);
		}
		else {
			hasSelectedCharacter1 = true;
			selectedWeapon = weaponName;
			ResetSlotsColors();
			SetSlotSelected(SlotNumber);
			DisableButtonsGivenSlotCharacter1(SlotNumber);
		}
	}
}

void UWExchangeEquipment::SetSelectedWeaponGivenSlotAndWeaponCharacter2(UButton* SlotNumber, EWeapon weaponName)
{
	if (hasSelectedCharacter2) {
		selectedWeapon = EWeapon::None;
		ResetSelectedCharacter();
	}
	else {
		if (hasSelectedCharacter1) {
			ChangeWeaponBetweenCharacters(character2, character1, weaponName);
		}
		else {
			hasSelectedCharacter2 = true;
			selectedWeapon = weaponName;
			ResetSlotsColors();
			SetSlotSelected(SlotNumber);
			DisableButtonsGivenSlotCharacter2(SlotNumber);
		}
	}
}

void UWExchangeEquipment::ChangeWeaponBetweenCharacters(APlayerCharacter* chara1, APlayerCharacter* chara2, EWeapon weaponName)
{
	bool S1 = chara1 == character2;
	if (selectedSlot1 == 1) {
		chara2->weapon1 = weaponName;
		SetWeaponInGivenSlotByBool(Slot1, Slot1_2, S1, weaponName);
	}
	else if (selectedSlot1 == 2) {
		chara2->weapon2 = weaponName;
		SetWeaponInGivenSlotByBool(Slot2, Slot2_2, S1, weaponName);
	}
	else if (selectedSlot1 == 3) {
		chara2->weapon3 = weaponName;
		SetWeaponInGivenSlotByBool(Slot3, Slot3_2, S1, weaponName);
	}
	else {
		chara2->weapon4 = weaponName;
		SetWeaponInGivenSlotByBool(Slot4, Slot4_2, S1, weaponName);
	}
	if (selectedSlot2 == 1) {
		chara1->weapon1 = selectedWeapon;
		SetWeaponInGivenSlotByBool(Slot1_2, Slot1, S1, selectedWeapon);
	}
	else if (selectedSlot2 == 2) {
		chara1->weapon2 = selectedWeapon;
		SetWeaponInGivenSlotByBool(Slot2_2, Slot2, S1, selectedWeapon);
	}
	else if (selectedSlot2 == 3) {
		chara1->weapon3 = selectedWeapon;
		SetWeaponInGivenSlotByBool(Slot3_2, Slot3, S1, selectedWeapon);
	}
	else {
		chara1->weapon4 = selectedWeapon;
		SetWeaponInGivenSlotByBool(Slot4_2, Slot4, S1, selectedWeapon);
	}
	selectedWeapon = EWeapon::None;
	ResetSelectedCharacter();
}

void UWExchangeEquipment::DisableButtons()
{
	if (character1 != nullptr) {
		Slot1->SetIsEnabled(false);
		DisableButtonGivenSlotAndWeapon(Slot2, character1->weapon2);
		DisableButtonGivenSlotAndWeapon(Slot3, character1->weapon3);
		DisableButtonGivenSlotAndWeapon(Slot4, character1->weapon4);
		DisableButtonGivenSlotAndItemNumber(SlotAmmo1, character1->ammo, character2->ammo);
		DisableButtonGivenSlotAndItemNumber(SlotFood1, character1->food, character2->food);
		DisableButtonGivenSlotAndItemNumber(SlotMedKit1, character1->medkit, character2->medkit);
	}
	else {
		EnableWeaponsCharacter1(false);
		DisableItemsButtons1();
	}
	if (character2 != nullptr) {
		Slot1_2->SetIsEnabled(false);
		DisableButtonGivenSlotAndWeapon(Slot2_2, character2->weapon2);
		DisableButtonGivenSlotAndWeapon(Slot3_2, character2->weapon3);
		DisableButtonGivenSlotAndWeapon(Slot4_2, character2->weapon4);
		DisableButtonGivenSlotAndItemNumber(SlotAmmo2, character2->ammo, character1->ammo);
		DisableButtonGivenSlotAndItemNumber(SlotFood2, character2->food, character1->food);
		DisableButtonGivenSlotAndItemNumber(SlotMedKit2, character2->medkit, character1->medkit);
	}
	else {
		EnableWeaponsCharacter2(false);
		DisableItemsButtons2();
	}
}

void UWExchangeEquipment::DisableItemsButtons1()
{
	SlotAmmo1->SetIsEnabled(false);
	SlotFood1->SetIsEnabled(false);
	SlotMedKit1->SetIsEnabled(false);
}

void UWExchangeEquipment::DisableItemsButtons2()
{
	SlotAmmo2->SetIsEnabled(false);
	SlotFood2->SetIsEnabled(false);
	SlotMedKit2->SetIsEnabled(false);
}

void UWExchangeEquipment::EnableWeaponsCharacter1(bool enableWeapon)
{
	if (enableWeapon && selectedWeapon == character1->PreferredWeapon) {
		Slot1->SetIsEnabled(enableWeapon);
		Slot2->SetIsEnabled(false);
		Slot3->SetIsEnabled(false);
		Slot4->SetIsEnabled(false);
	}
	else {
		Slot1->SetIsEnabled(false);
		Slot2->SetIsEnabled(enableWeapon);
		Slot3->SetIsEnabled(enableWeapon);
		Slot4->SetIsEnabled(enableWeapon);
	}
}

void UWExchangeEquipment::EnableWeaponsCharacter2(bool enableWeapon)
{
	if (enableWeapon && selectedWeapon == character2->PreferredWeapon) {
		Slot1_2->SetIsEnabled(enableWeapon);
		Slot2_2->SetIsEnabled(false);
		Slot3_2->SetIsEnabled(false);
		Slot4_2->SetIsEnabled(false);
	}
	else {
		Slot1_2->SetIsEnabled(false);
		Slot2_2->SetIsEnabled(enableWeapon);
		Slot3_2->SetIsEnabled(enableWeapon);
		Slot4_2->SetIsEnabled(enableWeapon);
	}
}

void UWExchangeEquipment::DisableButtonsGivenSlotCharacter1(UButton* SlotNumber)
{
	if (Slot1 != SlotNumber) {
		Slot1->SetIsEnabled(false);
	}
	if (Slot2 != SlotNumber) {
		Slot2->SetIsEnabled(false);
	}
	if (Slot3 != SlotNumber) {
		Slot3->SetIsEnabled(false);
	}
	if (Slot4 != SlotNumber) {
		Slot4->SetIsEnabled(false);
	}
	DisableItemsButtons1();
	DisableItemsButtons2();
	EnableWeaponsCharacter2(true);
}

void UWExchangeEquipment::DisableButtonsGivenSlotCharacter2(UButton* SlotNumber)
{
	if (Slot1_2 != SlotNumber) {
		Slot1_2->SetIsEnabled(false);
	}
	if (Slot2_2 != SlotNumber) {
		Slot2_2->SetIsEnabled(false);
	}
	if (Slot3_2 != SlotNumber) {
		Slot3_2->SetIsEnabled(false);
	}
	if (Slot4_2 != SlotNumber) {
		Slot4_2->SetIsEnabled(false);
	}
	DisableItemsButtons1();
	DisableItemsButtons2();
	EnableWeaponsCharacter1(true);
}

void UWExchangeEquipment::DisableButtonGivenSlotAndWeapon(UButton* SlotNumber, EWeapon weaponName)
{
	if (weaponName == EWeapon::None) {
		SlotNumber->SetIsEnabled(false);
	}
	else {
		SlotNumber->SetIsEnabled(true);
	}
}

void UWExchangeEquipment::DisableButtonGivenSlotAndItemNumber(UButton* SlotNumber, int numberCharacter1, int numberCharacter2)
{
	if (numberCharacter1 > 0 && numberCharacter2 < 3) {
		SlotNumber->SetIsEnabled(true);
	}
	else {
		SlotNumber->SetIsEnabled(false);
	}
}

void UWExchangeEquipment::DisableButtonsOnClickItem(bool enableFood, bool enableAmmo, bool enableMedKit)
{
	EnableWeaponsCharacter1(false);
	EnableWeaponsCharacter2(false);
	SlotFood1->SetIsEnabled(enableFood);
	SlotFood2->SetIsEnabled(enableFood);
	SlotAmmo1->SetIsEnabled(enableAmmo);
	SlotAmmo2->SetIsEnabled(enableAmmo);
	SlotMedKit1->SetIsEnabled(enableMedKit);
	SlotMedKit2->SetIsEnabled(enableMedKit);
}

void UWExchangeEquipment::SetWeaponInGivenSlotByBool(UButton* SlotNumber1, UButton* SlotNumber2, bool in1, EWeapon weaponName)
{
	if (in1) {
		SetWeaponImageInSlot(weaponName, SlotNumber1);
	}
	else {
		SetWeaponImageInSlot(weaponName, SlotNumber2);
	}
}
