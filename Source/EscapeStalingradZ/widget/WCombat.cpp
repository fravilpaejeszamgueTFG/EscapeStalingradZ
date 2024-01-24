// Fill out your copyright notice in the Description page of Project Settings.


#include "WCombat.h"
#include "WActions.h"
#include "buttons/Boton.h"
#include "EscapeStalingradZ/Grid/Grid.h"
#include "EscapeStalingradZ/player/actions/Command.h"
#include "EscapeStalingradZ/player/actions/ActionNormalFire.h"
#include "EscapeStalingradZ/player/actions/ActionHandToHand.h"
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

	buttonNormalFire->bIsEnabledDelegate.BindDynamic(this, &UWCombat::CanFireWeapon);
	buttonNormalFire->SynchronizeProperties();
	buttonSpreadFire->bIsEnabledDelegate.BindDynamic(this, &UWCombat::CanFireWeapon);
	buttonSpreadFire->SynchronizeProperties();



	APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (player != nullptr) {
		controller = player;
		character = player->playerchara;
	}
}

void UWCombat::OnClickNormalFire()
{
	command = NewObject<UActionNormalFire>(this);
	command->Execute(grid, character);
	controller->actions->command = NewObject<UActionNormalFire>(controller->actions);
	controller->actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
}

void UWCombat::OnClickSpreadFire()
{
}

void UWCombat::OnClickHandToHand()
{
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
	actions->SetVisibility(ESlateVisibility::Visible);
}

bool UWCombat::CanFireWeapon()
{
	if (character != nullptr && character->ammo>0) {
		if (character->useReadyWeapon && character->readyWeapon->multipleRange > 0) {
			return true;
		}
		if (!character->useReadyWeapon && character->readySecondaryWeapon->multipleRange > 0) {
			return true;
		}
	}
	return false;
}
