// Fill out your copyright notice in the Description page of Project Settings.


#include "WSelectStartTurn.h"
#include "Components/Button.h"
#include "EscapeStalingradZ/turn/Turn.h"

UWSelectStartTurn::UWSelectStartTurn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWSelectStartTurn::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonHuman->OnClicked.AddDynamic(this, &UWSelectStartTurn::OnClickButtonHuman);
	ButtonZombie->OnClicked.AddDynamic(this, &UWSelectStartTurn::OnClickButtonZombie);
}

void UWSelectStartTurn::OnClickButtonHuman()
{
	if (turn != nullptr) {
		SetVisibility(ESlateVisibility::Hidden);
		turn->SetNextCharacter();
	}
}

void UWSelectStartTurn::OnClickButtonZombie()
{
	if (turn != nullptr) {
		SetVisibility(ESlateVisibility::Hidden);
		turn->ZombiesStartTurn();
	}
}
