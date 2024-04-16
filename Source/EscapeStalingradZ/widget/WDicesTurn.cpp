// Fill out your copyright notice in the Description page of Project Settings.


#include "WDicesTurn.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "EscapeStalingradZ/turn/Turn.h"
#include "WSelectStartTurn.h"

UWDicesTurn::UWDicesTurn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWDicesTurn::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonConfirm->OnClicked.AddDynamic(this, &UWDicesTurn::OnClickButtonConfirm);
	ButtonRollAnimation->OnClicked.AddDynamic(this, &UWDicesTurn::OnClickButtonRollAnimation);
}

void UWDicesTurn::SetDices(int humanDie, int targetDie)
{
	currentNumber = humanDie;
	targetNumber = targetDie;
	NumberToBeat->SetText(FText::FromString(FString::FromInt(targetNumber)));
	if (currentNumber != targetNumber) {
		ButtonConfirmText->SetText(confirmText);
	}
	else {
		ButtonConfirmText->SetText(rollAgainText);
	}
	SetHumanDieImage(currentNumber);
}

void UWDicesTurn::OnClickButtonConfirm()
{
	DieNumber->SetVisibility(ESlateVisibility::Hidden);
	ButtonRollAnimation->SetVisibility(ESlateVisibility::Visible);
	ButtonConfirm->SetVisibility(ESlateVisibility::Hidden);
	SetVisibility(ESlateVisibility::Hidden);
	if (currentNumber > targetNumber) {
		CreateOrSetSelectStartTurn();
	}
	else if (currentNumber == targetNumber) {
		turn->Initiative();
	}
	else {
		turn->ZombiesStartTurn();
	}
}

void UWDicesTurn::CreateOrSetSelectStartTurn()
{
	if (selectStartTurnWidgetClass) {
		if (selectStartTurnWidget != nullptr) {
			selectStartTurnWidget->turn = turn;
			selectStartTurnWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			selectStartTurnWidget = CreateWidget<UWSelectStartTurn>(GetWorld(), selectStartTurnWidgetClass);
			if (selectStartTurnWidget != nullptr) {
				selectStartTurnWidget->turn = turn;
				selectStartTurnWidget->AddToViewport();
			}
		}
	}
}
