// Fill out your copyright notice in the Description page of Project Settings.


#include "WSearchDie.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/Grid/Grid.h"
#include "UserHUD.h"
#include "WPlayerInfo.h"

UWSearchDie::UWSearchDie(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWSearchDie::NativeConstruct()
{
    Super::NativeConstruct();

    ButtonConfirm->OnClicked.AddDynamic(this, &UWSearchDie::OnClickButtonConfirm);
    ButtonRollAnimation->OnClicked.AddDynamic(this, &UWSearchDie::OnClickButtonRollAnimation);
    hud = Cast<AUserHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void UWSearchDie::SetDie(int numberOfDie)
{
    currentNumber = numberOfDie;
    SetHumanDieImage(currentNumber);
    if (character != nullptr && character->grid != nullptr) {
        gridName = character->grid->name;
        SetSearchingObjects();
    }
}

void UWSearchDie::OnClickButtonConfirm()
{
    //TO-DO implementar objetivos en cada nivel
    for (const auto& item : objectsWon) {
        SetObjectWonToCharacter(item.Key, item.Value);
    }
    SetVisibility(ESlateVisibility::Hidden);
    if (hud != nullptr && hud->PlayerInfoWidget != nullptr) {
        hud->PlayerInfoWidget->UnhidePlayerInfoDuringSearch();
    }
}

void UWSearchDie::SetSearchingObjects()
{
    objectsWon.Empty();
    if (gridName == ScenarioName::FUBAR) {
        SetSearchingObjectsFubar();
    }
    else if (gridName == ScenarioName::AFRIEND) {
        SetSearchingObjectsAFriendWillBleed();
    }
    else if (gridName == ScenarioName::WAKEUP) {
        SetSearchingObjectsWakeUpTimeToDie();
    }
    else if (gridName == ScenarioName::STASH) {
        SetSearchingObjectsStash();
    }
    else {
        SetSearchingObjectsMoveAlong();
    }
}

void UWSearchDie::SetSearchingObjectsFubar()
{
    if (currentNumber > 2) {
        if (currentNumber <= 6) {
            objectsWon.Add(ObjectName::Ammo, 1);
        }
        else if (currentNumber <= 10) {
            objectsWon.Add(ObjectName::Food, 1);
        }
        else {
            objectsWon.Add(ObjectName::MedKit, 1);
        }
    }
}

void UWSearchDie::SetSearchingObjectsAFriendWillBleed()
{
    objectsWon.Add(ObjectName::SecondFavWeapon, 1);
    objectsWon.Add(ObjectName::Ammo, 1);
}

void UWSearchDie::SetSearchingObjectsWakeUpTimeToDie()
{
    //ubicaci�n o2 -> x= 0, por lo que x tiene que ser <= 1 para poder buscar ah�, ubicaci�n o -> x=4
    bool isO = true;
    if (character != nullptr && character->grid != nullptr) {
        AGrid* grid = character->grid;
        FIntPoint tile = grid->GetTileIndexFromLocation(character->GetActorLocation());
        if (tile.X <= 1) {
            isO = false;
        }
    }
    if (isO) {
        objectsWon.Add(ObjectName::WKnife, 1);
    }
    else {
        objectsWon.Add(ObjectName::WLuger, 1);
        objectsWon.Add(ObjectName::Ammo, 1);
    }
}

void UWSearchDie::SetSearchingObjectsStash()
{
    //TO-DO issue 88
    objectsWon.Add(ObjectName::Food, 1);
    objectsWon.Add(ObjectName::MedKit, 1);
    objectsWon.Add(ObjectName::Ammo, 1);
    objectsWon.Add(ObjectName::FavWeapon, 1);
}

void UWSearchDie::SetSearchingObjectsMoveAlong()
{
    if (currentNumber > 2) {
        if (currentNumber <= 9) {
            objectsWon.Add(ObjectName::Ammo, 1);
        }
        else if (currentNumber == 10) {
            objectsWon.Add(ObjectName::Food, 2);
        }
        else if (currentNumber == 11) {
            objectsWon.Add(ObjectName::Ammo, 2);
        }
        else {
            objectsWon.Add(ObjectName::MedKit, 1);
        }
    }
    else {
        objectsWon.Add(ObjectName::Food, 1);
    }
}

void UWSearchDie::SetObjectWonToCharacter(ObjectName name, int number)
{
    if (name == ObjectName::Ammo) {
        character->ammo += number;
        if (character->ammo > 3) {
            character->ammo = 3;
        }
    } 
    else if (name == ObjectName::Food) {
        character->food += number;
        if (character->food > 3) {
            character->food = 3;
        }
    }
    else if (name == ObjectName::MedKit) {
        character->medkit += number;
        if (character->medkit > 3) {
            character->medkit = 3;
        }
    }
    else if (name == ObjectName::FavWeapon) {
        character->weapon1 = character->PreferredWeapon;
    }
    else if (name == ObjectName::WLuger) {
        SetWeaponInFreeSlot(EWeapon::Luger);
    }
    else if (name == ObjectName::WKnife) {
        SetWeaponInFreeSlot(EWeapon::Knife);
    }
    else {
        if (character->isPrimaryPlayer) {
            //TO-DO pillar el otro personaje y conseguir su preferredWeapon para pasarselo a SetWeaponInFreeSlot
        }
        else {
            character->weapon1 = character->PreferredWeapon;
        }
    }
}

void UWSearchDie::SetWeaponInFreeSlot(EWeapon weaponName)
{
    if (character->weapon2 == EWeapon::None) {
        character->weapon2 = weaponName;
    }
    else if (character->weapon3 == EWeapon::None) {
        character->weapon3 = weaponName;
    }
    else {
        character->weapon4 = weaponName;
    }
}
