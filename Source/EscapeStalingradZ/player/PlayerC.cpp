// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerC.h"
#include "Components/WidgetComponent.h"
#include "EscapeStalingradZ/widget/WSelectMovementType.h"
#include "PlayerActions.h"


APlayerC::APlayerC()
{
	actions = CreateDefaultSubobject<UPlayerActions>("actions");

	SetActorHiddenInGame(false);
}

// Called when the game starts or when spawned
void APlayerC::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	EnableInput(this);

	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &APlayerC::LeftMouseClick);

	if (GetWorld()->GetName() == "AFriendWillBleed" || GetWorld()->GetName() == "WakeUp") {
		canExitTheRoom = false;
	}
}

void APlayerC::LeftMouseClick()
{
	actions->LeftMouseClick();
}

void APlayerC::SetMovementWidget()
{
	if (MovementClass) {
		if (Movement != nullptr) {
			Movement->character = playerchara;
			Movement->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			Movement = CreateWidget<UWSelectMovementType>(GetWorld(), MovementClass);
			if (Movement != nullptr) {
				Movement->character = playerchara;
				Movement->AddToViewport();
			}
		}
	}
}

void APlayerC::HideMovementWidget()
{
	Movement->SetVisibility(ESlateVisibility::Hidden);
}
