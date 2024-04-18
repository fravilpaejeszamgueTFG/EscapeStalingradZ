// Fill out your copyright notice in the Description page of Project Settings.


#include "WSearchDie.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/Grid/Grid.h"
#include "EscapeStalingradZ/player/PlayerC.h"
#include "EscapeStalingradZ/turn/Turn.h"
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
    if (gridName == ScenarioName::STASH) {
        if (numberOfSearch >= 3) {
            currentNumber = 12;
        }
        else {
            currentNumber = numberOfDie + numberOfSearch;
        }
        if (currentNumber > 12) {
            currentNumber = 12;
        }
    }
    else {
        currentNumber = numberOfDie;
    }
    unhidePlayerInfo = true;
    SetHumanDieImage(currentNumber);
    if (character != nullptr && character->grid != nullptr) {
        gridName = character->grid->name;
        SetSearchingObjects();
        SetActiveSearchObjectsWidget(GetNumberOfWidgetFromScenarioName());
    }
}

void UWSearchDie::OnClickButtonConfirm()
{
    DieNumber->SetVisibility(ESlateVisibility::Hidden);
    ButtonRollAnimation->SetVisibility(ESlateVisibility::Visible);
    ButtonConfirm->SetVisibility(ESlateVisibility::Hidden);
    numberOfSearch++;
    if (character != nullptr && character->grid != nullptr) {
        searchTileIndex = character->grid->currentSearchTile;
        character->grid->SetCurrentSearchTileSearched();
        APlayerC* player = Cast<APlayerC>(GetWorld()->GetFirstPlayerController());
        if (gridName == ScenarioName::STASH) {
            if (currentNumber > 10) {
                player->hasSecondaryObjective = true;
                character->grid->SetAllSearchTileSearched();
            }
        }
        else if (gridName == ScenarioName::WAKEUP) {
             AGrid* grid = character->grid;
             FIntPoint tile = grid->GetTileIndexFromLocation(character->GetActorLocation());
             if (tile.X > 1) { // tile.X > 1 -> o, <= 1 -> o2
                  player->CompletedPrimaryObjective();
             }
             else {
                  player->hasSecondaryObjective = true;
             }
        }
        else {
            player->hasSecondaryObjective = true;
        }
    }
    for (const auto& item : objectsWon) {
        SetObjectWonToCharacter(item.Key, item.Value);
    }
    SetVisibility(ESlateVisibility::Hidden);
    if (unhidePlayerInfo && hud != nullptr && hud->PlayerInfoWidget != nullptr) {
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
    //ubicación o2 -> x= 0, por lo que x tiene que ser <= 1 para poder buscar ahí, ubicación o -> x=4
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
    if (currentNumber < 3) {
        objectsWon.Add(ObjectName::Zombie);
    }
    else if (currentNumber == 9) {
        objectsWon.Add(ObjectName::Food, 1);
    }
    else if (currentNumber == 10) {
        objectsWon.Add(ObjectName::Ammo, 1);
    }
    else if (currentNumber > 10) {
        objectsWon.Add(ObjectName::FavWeapon, 1);
        objectsWon.Add(ObjectName::Food, 1);
        objectsWon.Add(ObjectName::MedKit, 1);
        objectsWon.Add(ObjectName::Ammo, 2);
    }
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
    switch (name) {
        case ObjectName::Ammo:
            character->ammo += number;
            if (character->ammo > 3) {
                character->ammo = 3;
            }
            break;
        case ObjectName::Food:
            character->food += number;
            if (character->food > 3) {
                character->food = 3;
            }
            break;
        case ObjectName::MedKit:
            character->medkit += number;
            if (character->medkit > 3) {
                character->medkit = 3;
            }
            break;
        case ObjectName::FavWeapon:
            if (character->isPrimaryPlayer) {
                character->weapon1 = character->PreferredWeapon;
            }
            else {
                if (hud != nullptr && hud->turn != nullptr) {
                    for (APlayerCharacter* chara : hud->turn->characters) {
                        if (chara->isPrimaryPlayer) {
                            SetWeaponInFreeSlot(chara->PreferredWeapon);
                            break;
                        }
                    }
                }
            }
            break;
        case ObjectName::WLuger:
            SetWeaponInFreeSlot(EWeapon::Luger);
            break;
        case ObjectName::WKnife:
            SetWeaponInFreeSlot(EWeapon::Knife);
            break;
        case ObjectName::SecondFavWeapon:
            if (character->isPrimaryPlayer) {
                if (hud != nullptr && hud->favoriteWeaponCharacterToFree != EWeapon::None) {
                    SetWeaponInFreeSlot(hud->favoriteWeaponCharacterToFree);
                }
            }
            else {
                character->weapon1 = character->PreferredWeapon;
            }
            break;
        case ObjectName::Zombie:
            if (searchTileIndex != FIntPoint(-1,-1)) {
                FIntPoint tileCharacter = character->grid->GetTileIndexFromLocation(character->GetActorLocation());
                if (hud != nullptr && hud->turn != nullptr) {
                    unhidePlayerInfo = false;
                    hud->turn->SpawnZombieInTile(searchTileIndex);
                }
            }
            break;
    }
}

int UWSearchDie::GetNumberOfWidgetFromScenarioName()
{
    if (gridName == ScenarioName::FUBAR) {
        return 0;
    }
    else if (gridName == ScenarioName::AFRIEND) {
        return 1;
    }
    else if (gridName == ScenarioName::WAKEUP) {
        if (character != nullptr && character->grid != nullptr) {
            AGrid* grid = character->grid;
            FIntPoint tile = grid->GetTileIndexFromLocation(character->GetActorLocation());
            if (tile.X <= 1) {
                return 3;
            }
        }
        return 2;
    }
    else if (gridName == ScenarioName::STASH) {
        return 4;
    }
    else {
        return 5;
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
