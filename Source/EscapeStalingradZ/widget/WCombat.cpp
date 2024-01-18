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

	APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (player != nullptr) {
		controller = player;
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
