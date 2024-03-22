// Fill out your copyright notice in the Description page of Project Settings.


#include "WCombat.h"
#include "WActions.h"
#include "buttons/Boton.h"
#include "EscapeStalingradZ/Grid/Grid.h"
#include "EscapeStalingradZ/player/actions/Command.h"
#include "EscapeStalingradZ/player/actions/ActionNormalFire.h"
#include "EscapeStalingradZ/player/actions/ActionHandToHand.h"
#include "EscapeStalingradZ/player/actions/ActionSpreadFire.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/player/PlayerActions.h"
#include "EscapeStalingradZ/player/PlayerC.h"
#include "Kismet/GameplayStatics.h"

UWCombat::UWCombat(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWCombat::NativeConstruct()
{
	Super::NativeConstruct();

	buttonNormalFire->OnClicked.AddDynamic(this, &UWCombat::OnClickNormalFire);
	buttonSpreadFire->OnClicked.AddDynamic(this, &UWCombat::OnClickSpreadFire);
	buttonHandToHand->OnClicked.AddDynamic(this, &UWCombat::OnClickHandToHand);
	goBack->OnClicked.AddDynamic(this, &UWCombat::GoBack);

	buttonNormalFire->VisibilityDelegate.BindDynamic(this, &UWCombat::CanFireWeapon);
	buttonNormalFire->SynchronizeProperties();
	buttonSpreadFire->VisibilityDelegate.BindDynamic(this, &UWCombat::CanFireWeaponInSpread);
	buttonSpreadFire->SynchronizeProperties();

	APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (player != nullptr) {
		controller = player;
		character = player->playerchara;
	}
}

void UWCombat::OnClickNormalFire()
{
	grid->deleteStatesFromTilesButSelected();
	command = NewObject<UActionNormalFire>(this);
	command->Execute(grid, character);
	controller->actions->command = NewObject<UActionNormalFire>(controller->actions);
	controller->actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWCombat::OnClickSpreadFire()
{
	grid->deleteStatesFromTilesButSelected();
	command = NewObject<UActionSpreadFire>(this);
	command->Execute(grid, character);
	controller->actions->command = NewObject<UActionSpreadFire>(controller->actions);
	controller->actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWCombat::OnClickHandToHand()
{
	grid->deleteStatesFromTilesButSelected();
	command = NewObject<UActionHandToHand>(this);
	command->Execute(grid, character);
	controller->actions->command = NewObject<UActionHandToHand>(controller->actions);
	controller->actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWCombat::GoBack()
{
	grid->deleteStatesFromTilesButSelected();
	controller->actions->command = nullptr;
	SetVisibility(ESlateVisibility::Hidden);
	actions->SetVisibleActionsAndOptions();
}

ESlateVisibility UWCombat::CanFireWeapon()
{
	if (character != nullptr && character->ammo>0) {
		if (character->useReadyWeapon && character->readyWeapon->multipleRange > 0) {
			return ESlateVisibility::Visible;
		}
		if (!character->useReadyWeapon && character->readySecondaryWeapon->multipleRange > 0) {
			return ESlateVisibility::Visible;
		}
	}
	return ESlateVisibility::Hidden;
}

ESlateVisibility UWCombat::CanFireWeaponInSpread()
{
	if (character != nullptr && character->ammo > 0) {
		if (character->useReadyWeapon && character->readyWeapon->multipleRange > 0 && character->readyWeapon->rateOfFire > 1) {
			return ESlateVisibility::Visible;
		}
		if (!character->useReadyWeapon && character->readySecondaryWeapon->multipleRange > 0 && character->readySecondaryWeapon->rateOfFire > 1) {
			return ESlateVisibility::Visible;
		}
	}
	return ESlateVisibility::Hidden;
}
