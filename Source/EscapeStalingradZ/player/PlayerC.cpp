// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerC.h"
#include "Components/WidgetComponent.h"
#include "EscapeStalingradZ/widget/WMovimiento.h"

// Called when the game starts or when spawned
void APlayerC::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	if (MovementClass) {
		Movement = CreateWidget<UWMovimiento>(GetWorld(), MovementClass);
		if (Movement != nullptr) {
			Movement->AddToViewport();
		}
	}
}

