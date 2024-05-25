// Fill out your copyright notice in the Description page of Project Settings.


#include "WSelectCharacter.h"
#include "Components/Button.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "UserHUD.h"

UWSelectCharacter::UWSelectCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWSelectCharacter::NativeConstruct()
{
    Super::NativeConstruct();

    ButtonLeft->OnClicked.AddDynamic(this, &UWSelectCharacter::OnClickButtonLeft);
    ButtonRight->OnClicked.AddDynamic(this, &UWSelectCharacter::OnClickButtonRight);
    ButtonConfirm->OnClicked.AddDynamic(this, &UWSelectCharacter::OnClickButtonConfirm);
}

void UWSelectCharacter::OnClickButtonLeft()
{
    if (index <= 0) {
        index = 3;
    }
    else {
        index--;
    }
    SetImages(false);
}

void UWSelectCharacter::OnClickButtonRight()
{
    if (index >= 3) {
        index = 0;
    }
    else {
        index++;
    }
    SetImages(true);
}

void UWSelectCharacter::OnClickButtonConfirm()
{
    if (character != nullptr) {
        if (index == 0) {
            character->characterChosen = AvailableCharacter::Becker;
        }
        else if (index == 1) {
            character->characterChosen = AvailableCharacter::Dasha;
        }
        else if (index == 2) {
            character->characterChosen = AvailableCharacter::Katyusha;
        }
        else {
            character->characterChosen = AvailableCharacter::Til;
        }
        character->SetPreferredWeaponByCharacter();
        character->SetImage();
    }
    SetVisibility(ESlateVisibility::Hidden);
    RemoveFromParent();
    if (hud != nullptr) {
        hud->StartGameAfterSelectCharacter();
    }
}

void UWSelectCharacter::SetImages(bool moveRight)
{
    DisableButtons();
    SetImagesBP(moveRight);
}

void UWSelectCharacter::DisableButtons()
{
    ButtonConfirm->SetIsEnabled(false);
    ButtonLeft->SetIsEnabled(false);
    ButtonRight->SetIsEnabled(false);
}

void UWSelectCharacter::EnableButtons()
{
    ButtonConfirm->SetIsEnabled(true);
    ButtonLeft->SetIsEnabled(true);
    ButtonRight->SetIsEnabled(true);
}
