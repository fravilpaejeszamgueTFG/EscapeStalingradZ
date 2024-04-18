// Fill out your copyright notice in the Description page of Project Settings.


#include "WSelectCharacterToFree.h"
#include "Components/Button.h"
#include "EscapeStalingradZ/character/CharacterToFree.h"
#include "UserHud.h"

UWSelectCharacterToFree::UWSelectCharacterToFree(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWSelectCharacterToFree::NativeConstruct()
{
    Super::NativeConstruct();

    ButtonLeft->OnClicked.AddDynamic(this, &UWSelectCharacterToFree::OnClickButtonLeft);
    ButtonRight->OnClicked.AddDynamic(this, &UWSelectCharacterToFree::OnClickButtonRight);
    ButtonConfirm->OnClicked.AddDynamic(this, &UWSelectCharacterToFree::OnClickButtonConfirm);
}

void UWSelectCharacterToFree::OnClickButtonLeft()
{
    if (index <= 0) {
        index = 2;
    }
    else {
        index--;
    }
    SetImages(false);
}

void UWSelectCharacterToFree::OnClickButtonRight()
{
    if (index >= 2) {
        index = 0;
    }
    else {
        index++;
    }
    SetImages(true);
}

void UWSelectCharacterToFree::OnClickButtonConfirm()
{
    if (character != nullptr) {
        if (index > 0 && index < charactersAvailable.Num()) {
            character->characterChosen = charactersAvailable[index];
        }
        character->SetImage();
    }
    SetVisibility(ESlateVisibility::Hidden);
    RemoveFromParent();
    if (hud != nullptr) {
        EWeapon weapon;
        if (character->characterChosen == AvailableCharacter::Becker) {
            weapon = EWeapon::MP40;
        }
        else if (character->characterChosen == AvailableCharacter::Dasha) {
            weapon = EWeapon::Nagant;
        }
        else if (character->characterChosen == AvailableCharacter::Katyusha) {
            weapon = EWeapon::MG34;
        }
        else {
            weapon = EWeapon::Axe;
        }
        hud->favoriteWeaponCharacterToFree = weapon;
        hud->StartGameAfterSelectCharacter();
    }
}

void UWSelectCharacterToFree::SetImages(bool moveRight)
{
    DisableButtons();
    SetImagesBP(moveRight);
}

void UWSelectCharacterToFree::DisableButtons()
{
    ButtonConfirm->SetIsEnabled(false);
    ButtonLeft->SetIsEnabled(false);
    ButtonRight->SetIsEnabled(false);
}

void UWSelectCharacterToFree::EnableButtons()
{
    ButtonConfirm->SetIsEnabled(true);
    ButtonLeft->SetIsEnabled(true);
    ButtonRight->SetIsEnabled(true);
}

void UWSelectCharacterToFree::SetCharactersAvailable(AvailableCharacter characterAlreadyChosen)
{
    if (characterAlreadyChosen != AvailableCharacter::Becker) {
        charactersAvailable.Add(AvailableCharacter::Becker);
    }
    if (characterAlreadyChosen != AvailableCharacter::Dasha) {
        charactersAvailable.Add(AvailableCharacter::Dasha);
    }
    if (characterAlreadyChosen != AvailableCharacter::Katyusha) {
        charactersAvailable.Add(AvailableCharacter::Katyusha);
    }
    if (characterAlreadyChosen != AvailableCharacter::Til) {
        charactersAvailable.Add(AvailableCharacter::Til);
    }
    SetInitialImage();
}

