// Fill out your copyright notice in the Description page of Project Settings.


#include "WDicesCombat.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "EscapeStalingradZ/zombies/Zombie.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "EscapeStalingradZ/widget/UserHUD.h"
#include "EscapeStalingradZ/widget/WPlayerInfo.h"
#include "EscapeStalingradZ/widget/WActions.h"
#include "TimerManager.h"
#include "EscapeStalingradZ/turn/Turn.h"
#include "EscapeStalingradZ/Grid/Grid.h"
#include "EscapeStalingradZ/misc/AnimatedTextAttack.h"

UWDicesCombat::UWDicesCombat(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWDicesCombat::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonConfirm->OnClicked.AddDynamic(this, &UWDicesCombat::OnClickButtonConfirm);
	ButtonRollAnimation->OnClicked.AddDynamic(this, &UWDicesCombat::OnClickButtonRollAnimation);

	hud = Cast<AUserHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void UWDicesCombat::SetDices(TArray<int> humanDices, int targetDie, bool isHandToHand)
{
	if (humanDices.Num() > 0) {
		if (hud != nullptr && hud->PlayerInfoWidget != nullptr) {
			if (attackInCovering) {
				hud->character = character;
				hud->PlayerInfoWidget->character = character;
				hud->PlayerInfoWidget->UpdateImages();
				hud->PlayerInfoWidget->HidePlayerInfoDuringCombat(zombie, true);
			}
			else {
				hud->PlayerInfoWidget->HidePlayerInfoDuringCombat(zombie, false);
			}
		}
		tileZombie = character->grid->GetTileIndexFromLocation(zombie->GetActorLocation());
		zombieStunned = zombie->isStunned;
		isAttackHandToHand = isHandToHand;
		humanDicesLeft = humanDices;
		currentNumber = humanDices[0];
		humanDicesLeft.RemoveSingle(currentNumber);
		numberToStun = targetDie;
		numberToKill = targetDie + 3;
		numberDiceLeft = humanDicesLeft.Num();
		NumberToKillText->SetText(FText::FromString(FString::FromInt(numberToKill)));
		NumberDiceLeftText->SetText(FText::FromString(FString::FromInt(numberDiceLeft + 1)));
		NumberToStunText->SetText(FText::FromString(FString::FromInt(numberToStun)));
		if (numberDiceLeft==0 || currentNumber >= numberToKill || (currentNumber == 1 && character->ammo == 1)) {
			ButtonConfirmText->SetText(confirmText);
		}
		else {
			ButtonConfirmText->SetText(rollAgainText);
		}
		SetHumanDieImage(currentNumber);
	}
}

void UWDicesCombat::NextDie()
{
	DieNumber->SetVisibility(ESlateVisibility::Hidden);
	ButtonRollAnimation->SetVisibility(ESlateVisibility::Visible);
	ButtonConfirm->SetVisibility(ESlateVisibility::Hidden);
	if (zombie->GetActorLocation().Z < -1000) {
		zombie->turn->SpawnWaitingZombies(tileZombie);
	}
	if (numberDiceLeft > 0) {
		//Tira el siguiente dado
		if (zombieStunned != zombie->isStunned) {
			numberToStun -= 3;
			numberToKill -= 3;
			NumberToKillText->SetText(FText::FromString(FString::FromInt(numberToKill)));
			NumberToStunText->SetText(FText::FromString(FString::FromInt(numberToStun)));
			zombieStunned = zombie->isStunned;
		}
		currentNumber = humanDicesLeft[0];
		humanDicesLeft.RemoveSingle(currentNumber);
		numberDiceLeft = humanDicesLeft.Num();
		NumberDiceLeftText->SetText(FText::FromString(FString::FromInt(numberDiceLeft + 1)));
		if (numberDiceLeft == 0 || currentNumber >= numberToKill || (currentNumber==1 && character->ammo ==1)) {
			ButtonConfirmText->SetText(confirmText);
		}
		else {
			ButtonConfirmText->SetText(rollAgainText);
		}
		SetHumanDieImage(currentNumber);
		SetVisibility(ESlateVisibility::Visible);
	}
	else {
		character->attacked = true;
		if (attackInCovering) {
			if (hud != nullptr && hud->PlayerInfoWidget != nullptr) {
				hud->character = character;
				hud->PlayerInfoWidget->character = character;
				hud->PlayerInfoWidget->UnhidePlayerInfoDuringCombat(true);
			}
			attackInCovering = false;
			character->typeOfCovering = CoveringType::NONE;
			if (zombie->GetActorLocation().Z > -1000) {
				zombie->charactersInCovering.Remove(character);
				zombie->CoveringAttack();
			}
			else {
				ATurn* turn = zombie->turn;
				turn->nextZombie();
			}
		}
		else {
			if (hud != nullptr && hud->PlayerInfoWidget != nullptr) {
				hud->PlayerInfoWidget->UnhidePlayerInfoDuringCombat(false);
				hud->PlayerInfoWidget->actionWidget->DisableAttack();
			}
		}
	}
}

void UWDicesCombat::AttackInHandToHand()
{
	bool zombieDied = zombie->ZombieHit(currentNumber, numberToStun);
	if (currentNumber == 2) {
		character->FriendlyFireGivenZombie(zombie);
	}
	if (zombieDied) {
		numberDiceLeft = 0;
		character->exp++;
	}
	//aqui iria animación de ataque si hay, poner el temporizador el tiempo de la animación
	GetWorld()->GetTimerManager().SetTimer(WaitTimer, this, &UWDicesCombat::NextDie, 2, false);
	UE_LOG(LogTemp, Warning, TEXT("aqui iria la animacion ataque cuerpo a cuerpo/normalFire"));
}

void UWDicesCombat::AttackInFire()
{
	if (currentNumber == 1) {
		character->ammo--;
		FVector pos = zombie->GetActorLocation();
		AAnimatedTextAttack* text = GetWorld()->SpawnActor<AAnimatedTextAttack>(textClass, pos, FRotator(0, 0, 0));
		if (text != nullptr) {
			text->SetAnimationText(FText::FromString("Fail"));
		}
		if (character->ammo <= 0) {
			numberDiceLeft = 0;
		}
		GetWorld()->GetTimerManager().SetTimer(WaitTimer, this, &UWDicesCombat::NextDie, 2, false);
		UE_LOG(LogTemp, Warning, TEXT("aqui iria la animacion fuego fallido"));
	}
	else {
		AttackInHandToHand();
	}
}

void UWDicesCombat::OnClickButtonConfirm()
{
	SetVisibility(ESlateVisibility::Hidden);
	if (isAttackHandToHand) {
		AttackInHandToHand();
	}
	else {
		AttackInFire();
	}
}
