// Fill out your copyright notice in the Description page of Project Settings.


#include "WSelectMovementType.h"
#include "WActions.h"
#include "buttons/Boton.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"


UWSelectMovementType::UWSelectMovementType(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UWSelectMovementType::NativeConstruct()
{

	Super::NativeConstruct();

	buttonRunning->OnClicked.AddDynamic(this, &UWSelectMovementType::OnClickRunning);
	buttonWalking->OnClicked.AddDynamic(this, &UWSelectMovementType::OnClickWalking);
	buttonStationary->OnClicked.AddDynamic(this, &UWSelectMovementType::OnClickStatic);
}

void UWSelectMovementType::OnClickRunning()
{
	character->mp = 8;
	character->typeOfMovement = MovementType::Running;
	CreateOrSetVisibleActionWidget(MovementType::Running);

}

void UWSelectMovementType::OnClickWalking()
{
	character->mp = 4;
	character->typeOfMovement = MovementType::Walking;
	CreateOrSetVisibleActionWidget(MovementType::Walking);
}

void UWSelectMovementType::OnClickStatic()
{
	character->mp = 4;
	character->typeOfMovement = MovementType::Stationary;
	CreateOrSetVisibleActionWidget(MovementType::Stationary);
}

void UWSelectMovementType::CreateOrSetVisibleActionWidget(MovementType type)
{
	if (actionWidgetClass) {
		if (actionWidget != nullptr) {
			actionWidget->character = character;
			actionWidget->DisableButtonByMovementType(type);
			actionWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			actionWidget = CreateWidget<UWActions>(GetWorld(), actionWidgetClass);
			if (actionWidget != nullptr) {
				actionWidget->character = character;
				actionWidget->turn = turn;
				actionWidget->DisableButtonByMovementType(type);
				actionWidget->AddToViewport();
			}
		}
		SetVisibility(ESlateVisibility::Hidden);
	}
}
