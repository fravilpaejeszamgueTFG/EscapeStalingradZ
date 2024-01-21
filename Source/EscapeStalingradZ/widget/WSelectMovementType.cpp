// Fill out your copyright notice in the Description page of Project Settings.


#include "WSelectMovementType.h"
#include "buttons/Boton.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "UserHUD.h"


UWSelectMovementType::UWSelectMovementType(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UWSelectMovementType::NativeConstruct()
{

	Super::NativeConstruct();

	buttonRunning->OnClicked.AddDynamic(this, &UWSelectMovementType::OnClickRunning);
	buttonWalking->OnClicked.AddDynamic(this, &UWSelectMovementType::OnClickWalking);
	buttonStationary->OnClicked.AddDynamic(this, &UWSelectMovementType::OnClickStatic);

	hud = Cast<AUserHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void UWSelectMovementType::OnClickRunning()
{
	character->mp = 8;
	character->typeOfMovement = MovementType::Running;
	SetVisibility(ESlateVisibility::Hidden);
	hud->CreateOrSetPlayerInfo();
}

void UWSelectMovementType::OnClickWalking()
{
	character->mp = 4;
	character->typeOfMovement = MovementType::Walking;
	SetVisibility(ESlateVisibility::Hidden);
	hud->CreateOrSetPlayerInfo();
}

void UWSelectMovementType::OnClickStatic()
{
	character->mp = 4;
	character->typeOfMovement = MovementType::Stationary;
	SetVisibility(ESlateVisibility::Hidden);
	hud->CreateOrSetPlayerInfo();
}
