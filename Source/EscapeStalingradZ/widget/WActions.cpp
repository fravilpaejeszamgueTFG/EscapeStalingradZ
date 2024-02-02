// Fill out your copyright notice in the Description page of Project Settings.


#include "WActions.h"
#include "WMovimiento.h"
#include "WCombat.h"
#include "WOtherActions.h"
#include "buttons/Boton.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/Grid/Grid.h"
#include "EscapeStalingradZ/player/PlayerActions.h"
#include "EscapeStalingradZ/player/PlayerC.h"
#include "EscapeStalingradZ/turn/Turn.h"
#include "Kismet/GameplayStatics.h"
#include "EscapeStalingradZ/widget/UserHUD.h"

UWActions::UWActions(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UWActions::NativeConstruct()
{

	Super::NativeConstruct();


	buttonMovement->OnClicked.AddDynamic(this, &UWActions::OnClickMovement);
	buttonCombat->OnClicked.AddDynamic(this, &UWActions::OnClickCombat);
	buttonActions->OnClicked.AddDynamic(this, &UWActions::OnClickActions);
	endTurn->OnClicked.AddDynamic(this, &UWActions::EndTurn);

	APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (player != nullptr) {
		controller = player;
		character = player->playerchara;
	}
	hud = Cast<AUserHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void UWActions::OnClickMovement()
{
	if (movementWidgetClass) {
		if (movementWidget != nullptr) {
			movementWidget->character = character;
			movementWidget->DisableButtonByMovementType(character->typeOfMovement);
			movementWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else {
			movementWidget = CreateWidget<UWMovimiento>(GetWorld(), movementWidgetClass);
			if (movementWidget != nullptr) {
				movementWidget->character = character;
				movementWidget->turn = turn;
				movementWidget->grid = grid;
				movementWidget->actions = this;
				movementWidget->DisableButtonByMovementType(character->typeOfMovement);
				movementWidget->AddToViewport();
			}
		}
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWActions::OnClickCombat()
{
	if (combatWidgetClass) {
		if (combatWidget != nullptr) {
			combatWidget->character = character;
			combatWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else {
			combatWidget = CreateWidget<UWCombat>(GetWorld(), combatWidgetClass);
			if (combatWidget != nullptr) {
				combatWidget->character = character;
				combatWidget->turn = turn;
				combatWidget->grid = grid;
				combatWidget->actions = this;
				combatWidget->AddToViewport();
			}
		}
		character->getArcOfFire();
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWActions::OnClickActions()
{
	if (actionWidgetClass) {
		if (actionWidget != nullptr) {
			actionWidget->character = character;
			actionWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else {
			actionWidget = CreateWidget<UWOtherActions>(GetWorld(), actionWidgetClass);
			if (actionWidget != nullptr) {
				actionWidget->character = character;
				actionWidget->turn = turn;
				actionWidget->grid = grid;
				actionWidget->actions = this;
				actionWidget->AddToViewport();
			}
		}
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWActions::EndTurn()
{
	if (turn != nullptr) {
		grid->deleteStatesFromTiles();
		turn->nextCharacter();
		controller->actions->command = nullptr;
	}
	SetVisibility(ESlateVisibility::Hidden);
	hud->HidePlayerInfo();
}

void UWActions::HideWidgets()
{
	if (combatWidget != nullptr && combatWidget->Visibility != ESlateVisibility::Hidden) {
		combatWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (actionWidget != nullptr && actionWidget->Visibility != ESlateVisibility::Hidden) {
		actionWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (movementWidget != nullptr && movementWidget->Visibility != ESlateVisibility::Hidden) {
		movementWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWActions::DisableButtonByMovementType(MovementType type)
{
	if (type == MovementType::Running) {
		buttonMovement->SetVisibility(ESlateVisibility::Visible);
	}
	else if (type == MovementType::Stationary) {
		buttonMovement->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		buttonMovement->SetVisibility(ESlateVisibility::Visible);
	}
}
