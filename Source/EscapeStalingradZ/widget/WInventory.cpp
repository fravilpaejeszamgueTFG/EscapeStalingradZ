// Fill out your copyright notice in the Description page of Project Settings.


#include "WInventory.h"
#include "Kismet/GameplayStatics.h"
#include "UserHUD.h"
#include "WPlayerInfo.h"
#include "Components/Button.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/weapon/Weapon.h"
#include "Components/TextBlock.h"

UWInventory::UWInventory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWInventory::NativeConstruct()
{
	Super::NativeConstruct();

	Back->OnClicked.AddDynamic(this, &UWInventory::OnClickGoBack);
	Slot1->OnClicked.AddDynamic(this, &UWInventory::OnClickSlot1);
	Slot2->OnClicked.AddDynamic(this, &UWInventory::OnClickSlot2);
	Slot3->OnClicked.AddDynamic(this, &UWInventory::OnClickSlot3);
	Slot4->OnClicked.AddDynamic(this, &UWInventory::OnClickSlot4);
	SlotReadyWeapon->OnClicked.AddDynamic(this, &UWInventory::OnClickSlotReadyWeapon);
	SlotSecondaryWeapon->OnClicked.AddDynamic(this, &UWInventory::OnClickSlotSecondaryWeapon);

}

void UWInventory::OnClickSlot1()
{
	//hacer esto en los otros slots y pasar a funcion por parametro, tambien bloquear armas 
	if (character != nullptr) {
		SetSelectedWeaponGivenSlotAndWeapon(Slot1, character->weapon1);
	}
}

void UWInventory::OnClickSlot2()
{
	if (character != nullptr) {
		SetSelectedWeaponGivenSlotAndWeapon(Slot2, character->weapon2);
	}
}

void UWInventory::OnClickSlot3()
{
	if (character != nullptr) {
		SetSelectedWeaponGivenSlotAndWeapon(Slot3, character->weapon3);
	}
}

void UWInventory::OnClickSlot4()
{
	if (character != nullptr) {
		SetSelectedWeaponGivenSlotAndWeapon(Slot4, character->weapon4);
	}
}

void UWInventory::OnClickSlotReadyWeapon()
{
	if (character != nullptr && hasSelectedWeapon) {
		character->mp -= 2;
		SetWeaponSlotReadyImageByGivenWeapon(selectedWeapon);
		if (character->readySecondaryWeapon->weaponName == selectedWeapon) {
			character->readySecondaryWeapon->SetPropiertiesByName(EWeapon::None);
			SetWeaponSlotSecondaryImageByGivenWeapon(EWeapon::None);
			character->useReadyWeapon = true;
			if (hud != nullptr && hud->PlayerInfoWidget != nullptr) {
				hud->PlayerInfoWidget->SetUseReadyWeaponColor();
				hud->PlayerInfoWidget->UpdateImages();
			}
		}
		character->readyWeapon->SetPropiertiesByName(selectedWeapon);
		DisableButtons();
		ResetSelectedWeapon();
		if (hud != nullptr && hud->PlayerInfoWidget != nullptr) {
			hud->PlayerInfoWidget->UpdateImages();
		}
	}
}

void UWInventory::OnClickSlotSecondaryWeapon()
{
	if (character != nullptr && hasSelectedWeapon) {
		character->mp -= 2;
		SetWeaponSlotSecondaryImageByGivenWeapon(selectedWeapon);
		if (character->readyWeapon->weaponName == selectedWeapon) {
			character->readyWeapon->SetPropiertiesByName(EWeapon::None);
			SetWeaponSlotReadyImageByGivenWeapon(EWeapon::None);
		}
		character->readySecondaryWeapon->SetPropiertiesByName(selectedWeapon);
		DisableButtons();
		ResetSelectedWeapon();
		if (hud != nullptr && hud->PlayerInfoWidget != nullptr) {
			hud->PlayerInfoWidget->UpdateImages();
		}
	}
}

void UWInventory::OnClickGoBack()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UWInventory::SetTextNumbers()
{
	if (character != nullptr) {
		AmmoNumber->SetText(FText::FromString(FString("x").Append(FString::FromInt(character->ammo))));
		FoodNumber->SetText(FText::FromString(FString("x").Append(FString::FromInt(character->food))));
		MedKitNumber->SetText(FText::FromString(FString("x").Append(FString::FromInt(character->medkit))));
	}
}

void UWInventory::ResetSelectedWeapon()
{
	hasSelectedWeapon = false;
	selectedWeapon = EWeapon::None;
	ResetSlotsColors();
}

void UWInventory::SetSelectedWeaponGivenSlotAndWeapon(UButton* SlotNumber, EWeapon weaponName)
{
	if (hasSelectedWeapon) {
		if (selectedWeapon == weaponName) {
			ResetSelectedWeapon();
			DisableReadyAndSecondaryButtons();
		}
		else {
			selectedWeapon = weaponName;
			ResetSlotsColors();
			SetSlotSelected(SlotNumber);
			ActivateReadyAndSecondaryIfProceed(weaponName);
		}
	}
	else {
		hasSelectedWeapon = true;
		selectedWeapon = weaponName;
		SetSlotSelected(SlotNumber);
		ActivateReadyAndSecondaryIfProceed(weaponName);
	}
}

void UWInventory::DisableButtons()
{
	if (character != nullptr && (character->mp >= 2 || inBetweenScenarios)) {
		if (character->weapon1 == EWeapon::None) {
			Slot1->SetIsEnabled(false);
		}
		else {
			Slot1->SetIsEnabled(true);
		}
		if (character->weapon2 == EWeapon::None) {
			Slot2->SetIsEnabled(false);
		}
		else {
			Slot2->SetIsEnabled(true);
		}
		if (character->weapon3 == EWeapon::None) {
			Slot3->SetIsEnabled(false);
		}
		else {
			Slot3->SetIsEnabled(true);
		}
		Slot4->SetIsEnabled(true);// este slot siempre se puede seleccionar para cambiar ya que tendra los puños por defecto
	}
	else {
		Slot1->SetIsEnabled(false);
		Slot2->SetIsEnabled(false);
		Slot3->SetIsEnabled(false);
		Slot4->SetIsEnabled(false);
	}
	//Se activan cuando se elija un arma para poder cambiarlo
	DisableReadyAndSecondaryButtons();
}

void UWInventory::DisableReadyAndSecondaryButtons()
{
	SlotReadyWeapon->SetIsEnabled(false);
	SlotSecondaryWeapon->SetIsEnabled(false);
}

void UWInventory::ActivateReadyAndSecondaryIfProceed(EWeapon weaponName)
{
	if (character != nullptr) {
		if (character->readyWeapon->weaponName == weaponName) {
			SlotReadyWeapon->SetIsEnabled(false);
		}
		else {
			SlotReadyWeapon->SetIsEnabled(true);
		}
		if (character->readySecondaryWeapon->weaponName == weaponName) {
			SlotSecondaryWeapon->SetIsEnabled(false);
		}
		else {
			if (character->readySecondaryWeapon->GetIfWeaponIsTwoHandByName(weaponName)) {
				SlotSecondaryWeapon->SetIsEnabled(true);
			}
			else {
				SlotSecondaryWeapon->SetIsEnabled(false);
			}
		}
	}
}
