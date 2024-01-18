// Fill out your copyright notice in the Description page of Project Settings.


#include "WActions.h"
#include "WMovimiento.h"
#include "buttons/Boton.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/Grid/Grid.h"
#include "EscapeStalingradZ/player/PlayerActions.h"
#include "EscapeStalingradZ/player/PlayerC.h"
#include "EscapeStalingradZ/turn/Turn.h"
#include "Kismet/GameplayStatics.h"

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
	}
}

void UWActions::OnClickMovement()
{
	if (movementWidgetClass) {
		if (movementWidget != nullptr) {
			movementWidget->character = character;
			movementWidget->DisableButtonByMovementType(movementType);
			movementWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			movementWidget = CreateWidget<UWMovimiento>(GetWorld(), movementWidgetClass);
			if (movementWidget != nullptr) {
				movementWidget->character = character;
				movementWidget->turn = turn;
				movementWidget->grid = grid;
				movementWidget->DisableButtonByMovementType(movementType);
				movementWidget->AddToViewport();
			}
		}
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWActions::OnClickCombat()
{
}

void UWActions::OnClickActions()
{
}

void UWActions::EndTurn()
{
	if (turn != nullptr) {
		grid->deleteStatesFromTiles();
		turn->nextCharacter();
		controller->actions->command = nullptr;
	}
	SetVisibility(ESlateVisibility::Hidden);
}

void UWActions::DisableButtonByMovementType(MovementType type)
{
	movementType = type;
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
