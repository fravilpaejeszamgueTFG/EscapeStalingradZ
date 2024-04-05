// Fill out your copyright notice in the Description page of Project Settings.

#include "WPlayerInfo.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/grid/Grid.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Overlay.h"
#include "Components/Button.h"
#include "WActions.h"
#include "WObjectives.h"
#include "WOptions.h"
#include "Styling/SlateTypes.h"
#include "EscapeStalingradZ/grid/Grid.h"
#include "EscapeStalingradZ/zombies/Zombie.h"

UWPlayerInfo::UWPlayerInfo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWPlayerInfo::NativeConstruct()
{

	Super::NativeConstruct();

	HealthBar->PercentDelegate.BindDynamic(this, &UWPlayerInfo::SetHealthBar);
	HealthBar->SynchronizeProperties();
	MPBar->PercentDelegate.BindDynamic(this, &UWPlayerInfo::SetMPBar);
	MPBar->SynchronizeProperties();
	health->TextDelegate.BindDynamic(this, &UWPlayerInfo::SetHealthText);
	health->SynchronizeProperties();
	MP->TextDelegate.BindDynamic(this, &UWPlayerInfo::SetMPText);
	MP->SynchronizeProperties();
	KillNumber->TextDelegate.BindDynamic(this, &UWPlayerInfo::SetKillNumberText);
	KillNumber->SynchronizeProperties();
	AmmoNumber->TextDelegate.BindDynamic(this, &UWPlayerInfo::SetAmmoNumberText);
	AmmoNumber->SynchronizeProperties();
	FoodNumber->TextDelegate.BindDynamic(this, &UWPlayerInfo::SetFoodNumberText);
	FoodNumber->SynchronizeProperties();
	MedKitNumber->TextDelegate.BindDynamic(this, &UWPlayerInfo::SetMedKitNumberText);
	MedKitNumber->SynchronizeProperties();
	SecondaryWeapon_Overlay->VisibilityDelegate.BindDynamic(this, &UWPlayerInfo::SetVisibilitySecondaryWeapon);
	SecondaryWeapon_Overlay->SynchronizeProperties();

	ButtonReadyWeapon->OnClicked.AddDynamic(this, &UWPlayerInfo::OnClickReadyWeapon);
	ButtonSecondaryWeapon->OnClicked.AddDynamic(this, &UWPlayerInfo::OnClickSecondaryWeapon);
	
}

float UWPlayerInfo::SetHealthBar()
{
	if (character != nullptr) {
		return character->health / 4.f;
	}
	return 0;
}

float UWPlayerInfo::SetMPBar()
{
	if (character != nullptr) {
		if (character->typeOfMovement == Running) {
			return character->mp / 8.f;
		}
		return character->mp / 4.f;
	}
	return 0;
}

FText UWPlayerInfo::SetHealthText()
{
	if (character != nullptr) {
		FString healthText = FString::FromInt(character->health);
		healthText.Append(FString("/4"));
		return FText::FromString(healthText);
	}
	return FText();
}

FText UWPlayerInfo::SetMPText()
{
	if (character != nullptr) {
		FString mpText = FString::FromInt(character->mp); 
		if (character->typeOfMovement == Running) {
			mpText.Append(FString("/8"));
		}
		else {
			mpText.Append(FString("/4"));
		}
		return FText::FromString(mpText);
	}
	return FText();
}

FText UWPlayerInfo::SetKillNumberText()
{
	if (character != nullptr) {
		return FText::FromString(FString("x").Append(FString::FromInt(character->exp)));
	}
	return FText();
}

FText UWPlayerInfo::SetAmmoNumberText()
{
	if (character != nullptr) {
		return FText::FromString(FString("x").Append(FString::FromInt(character->ammo)));
	}
	return FText();
}

FText UWPlayerInfo::SetFoodNumberText()
{
	if (character != nullptr) {
		return FText::FromString(FString("x").Append(FString::FromInt(character->food)));
	}
	return FText();
}

FText UWPlayerInfo::SetMedKitNumberText()
{
	if (character != nullptr) {
		return FText::FromString(FString("x").Append(FString::FromInt(character->medkit)));
	}
	return FText();
}

ESlateVisibility UWPlayerInfo::SetVisibilitySecondaryWeapon()
{
	if (character != nullptr && character->readySecondaryWeapon->weaponName != EWeapon::None) {
		return ESlateVisibility::SelfHitTestInvisible;
	}
	return ESlateVisibility::Hidden;
}

void UWPlayerInfo::UpdateImages()
{
	SetCharacterImage();
	SetReadyWImage();
	SetSecondaryWImage();
	if (character->useReadyWeapon) {
		SetUseReadyWeaponColor();
	}
	else {
		SetUseSecondaryWeaponColor();
	}
}

void UWPlayerInfo::SetVisibleActionWidget()
{
	if (actionWidget != nullptr) {
		actionWidget->character = character;
		actionWidget->DisableButtonByMovementType(character->typeOfMovement);
		actionWidget->DisableButtonIfCharacterIsInDirectContact();
		actionWidget->HideWidgets();
		actionWidget->turn = turn;
		actionWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (character->mp == 0) {
			actionWidget->DisableActions();
		}
		SetVisibleOptionsWidget();
	}
}

void UWPlayerInfo::SetVisibleOptionsWidget()
{
	if (optionsWidget != nullptr) {
		optionsWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UWPlayerInfo::SetVisibleObjectivesWidget()
{
	if (objectiveWidget != nullptr) {
		objectiveWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UWPlayerInfo::OnClickReadyWeapon()
{
	if (character != nullptr) {
		character->grid->deleteStatesFromTilesButSelected();
		character->useReadyWeapon = true;
		SetUseReadyWeaponColor();
		if (actionWidget != nullptr) {
			actionWidget->UpdateCoveringAttackWidget();
		}
	}
}

void UWPlayerInfo::OnClickSecondaryWeapon()
{
	if (character != nullptr) {
		character->grid->deleteStatesFromTilesButSelected();
		character->useReadyWeapon = false;
		SetUseSecondaryWeaponColor();
		if (actionWidget != nullptr) {
			actionWidget->UpdateCoveringAttackWidget();
		}
	}
}

void UWPlayerInfo::HidePlayerInfoDuringCombat(AZombie* zombie, bool inCovering)
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	optionsWidget->SetVisibility(ESlateVisibility::Hidden);
	actionWidget->SetVisibility(ESlateVisibility::Hidden);
	actionWidget->HideWidgets();
	SetZombieTileHovered(zombie);
	if (inCovering) {
		AGrid* grid = character->grid;
		grid->AddTileState(grid->GetTileIndexFromLocation(character->GetActorLocation()), TileState::Selected);
	}
}

void UWPlayerInfo::UnhidePlayerInfoDuringCombat(bool inCovering)
{
	if (inCovering) {
		SetVisibility(ESlateVisibility::Hidden);
		character->grid->deleteStatesFromTiles();
	}
	else {
		SetVisibility(ESlateVisibility::Visible);
		character->grid->deleteStatesFromTilesButSelected();
		optionsWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SetVisibleActionWidget();
	}
}

void UWPlayerInfo::SetZombieTileHovered(AZombie* zombie)
{
	TArray<FIntPoint> tiles = TArray<FIntPoint>();
	tiles.Add(character->grid->GetTileIndexFromLocation(zombie->GetActorLocation()));
	character->grid->DeleteStatesFromTilesButGiven(tiles);
}

void UWPlayerInfo::HidePlayerInfoDuringSearch()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	optionsWidget->SetVisibility(ESlateVisibility::Hidden);
	actionWidget->SetVisibility(ESlateVisibility::Hidden);
	actionWidget->HideWidgets();
}

void UWPlayerInfo::UnhidePlayerInfoDuringSearch()
{
	SetVisibility(ESlateVisibility::Visible);
	character->grid->deleteStatesFromTilesButSelected();
	optionsWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SetVisibleActionWidget();
	actionWidget->DisableActions();
}

void UWPlayerInfo::HideOptionsDuringActions()
{
	if (optionsWidget != nullptr) {
		optionsWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
