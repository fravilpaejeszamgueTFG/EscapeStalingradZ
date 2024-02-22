// Fill out your copyright notice in the Description page of Project Settings.


#include "WReleaseLock.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "EscapeStalingradZ/turn/Turn.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "UserHUD.h"

UWReleaseLock::UWReleaseLock(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWReleaseLock::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonConfirm->OnClicked.AddDynamic(this, &UWReleaseLock::OnClickButtonConfirm);
	ButtonRollAnimation->OnClicked.AddDynamic(this, &UWReleaseLock::OnClickButtonRollAnimation);
}

void UWReleaseLock::SetDicesNumbers()
{
	int humanDie = FMath::RandRange(1, 12);
	int targetDie = character->GetPrimaryHitHandToHand();
	if (character->typeOfMovement == MovementType::Walking) {
		targetDie += 2;
	}
	else if (character->typeOfMovement == MovementType::Running) {
		targetDie += 4;
	}
	SetDices(humanDie, targetDie);
}

void UWReleaseLock::SetDices(int humanDie, int targetDie)
{
	currentNumber = humanDie;
	targetNumber = targetDie;
	NumberToBeat->SetText(FText::FromString(FString::FromInt(targetNumber)));
	SetHumanDieImage(currentNumber);
}

void UWReleaseLock::OnClickButtonConfirm()
{
	DieNumber->SetVisibility(ESlateVisibility::Hidden);
	ButtonRollAnimation->SetVisibility(ESlateVisibility::Visible);
	ButtonConfirm->SetVisibility(ESlateVisibility::Hidden);
	SetVisibility(ESlateVisibility::Hidden);
	if (currentNumber >= targetNumber) {
		character->isLocked = false;
		AUserHUD* hud = Cast<AUserHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		if (hud != nullptr) {
			hud->CreateOrSetPlayerInfo();
		}
	}
	else {
		turn->nextCharacter();
	}
}
