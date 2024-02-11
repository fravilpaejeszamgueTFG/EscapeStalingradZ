// Fill out your copyright notice in the Description page of Project Settings.


#include "WDiceSpreadCombat.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "EscapeStalingradZ/zombies/Zombie.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/grid/Grid.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "EscapeStalingradZ/widget/UserHUD.h"
#include "EscapeStalingradZ/widget/WPlayerInfo.h"
#include "TimerManager.h"
#include "WSelectObjectiveSpreadFire.h"
#include "EscapeStalingradZ/player/actions/Command.h"
#include "EscapeStalingradZ/player/actions/ActionNewObjectiveSpreadFire.h"
#include "EscapeStalingradZ/player/PlayerActions.h"
#include "EscapeStalingradZ/player/PlayerC.h"

UWDiceSpreadCombat::UWDiceSpreadCombat(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWDiceSpreadCombat::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonConfirm->OnClicked.AddDynamic(this, &UWDiceSpreadCombat::OnClickButtonConfirm);
	ButtonRollAnimation->OnClicked.AddDynamic(this, &UWDiceSpreadCombat::OnClickButtonRollAnimation);

	hud = Cast<AUserHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void UWDiceSpreadCombat::SetDices(TArray<int> humanDices, int targetDie)
{
	if (humanDices.Num() > 0) {
		if (hud != nullptr && hud->PlayerInfoWidget != nullptr) {
			hud->PlayerInfoWidget->HidePlayerInfoDuringCombat(zombie);
		}
		tileZombie = character->grid->GetTileIndexFromLocation(zombie->GetActorLocation());
		humanDicesLeft = humanDices;
		currentNumber = humanDices[0];
		humanDicesLeft.RemoveSingle(currentNumber);
		numberToStun = targetDie;
		numberToKill = targetDie + 3;
		numberDiceLeft = humanDicesLeft.Num();
		NumberToKillText->SetText(FText::FromString(FString::FromInt(numberToKill)));
		NumberDiceLeftText->SetText(FText::FromString(FString::FromInt(numberDiceLeft + 1)));
		NumberToStunText->SetText(FText::FromString(FString::FromInt(numberToStun)));
		SetHumanDieImage(currentNumber);
	}
}

void UWDiceSpreadCombat::NextDie(int targetDie)
{
	if (SelectObjectiveWidget != nullptr) {
		SelectObjectiveWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (hud != nullptr && hud->PlayerInfoWidget != nullptr) {
		hud->PlayerInfoWidget->SetZombieTileHovered(zombie);
	}
	DieNumber->SetVisibility(ESlateVisibility::Hidden);
	ButtonRollAnimation->SetVisibility(ESlateVisibility::Visible);
	ButtonConfirm->SetVisibility(ESlateVisibility::Hidden);
	tileZombie = character->grid->GetTileIndexFromLocation(zombie->GetActorLocation());
	numberToStun = targetDie;
	numberToKill = targetDie + 3;
	currentNumber = humanDicesLeft[0];
	humanDicesLeft.RemoveSingle(currentNumber);
	numberDiceLeft = humanDicesLeft.Num();
	NumberToKillText->SetText(FText::FromString(FString::FromInt(numberToKill)));
	NumberDiceLeftText->SetText(FText::FromString(FString::FromInt(numberDiceLeft + 1)));
	NumberToStunText->SetText(FText::FromString(FString::FromInt(numberToStun)));
	SetHumanDieImage(currentNumber);
	SetVisibility(ESlateVisibility::Visible);
}

void UWDiceSpreadCombat::AttackSpreadFire()
{
	if (currentNumber == 1) {
		character->ammo--;
		UE_LOG(LogTemp, Warning, TEXT("Se me ha encasquillao"));
		bool withoutAmmo = false;
		if (character->ammo <= 0) {
			UE_LOG(LogTemp, Warning, TEXT("Sin balas. Perfecto"));
			withoutAmmo = true;
		}
		if (withoutAmmo) {
			GetWorld()->GetTimerManager().SetTimer(WaitTimer, this, &UWDiceSpreadCombat::EndAttack, 3, false);
			UE_LOG(LogTemp, Warning, TEXT("aqui iria la animacion fuego fallido + fin ataque"));
		}
		else {
			GetWorld()->GetTimerManager().SetTimer(WaitTimer, this, &UWDiceSpreadCombat::SelectObjetiveSpreadFire, 3, false);
			UE_LOG(LogTemp, Warning, TEXT("aqui iria la animacion fuego fallido"));
		}
	}
	else {
		bool zombieDied = zombie->ZombieHit(currentNumber, numberToStun);
		if (currentNumber == 2) {
			character->FriendlyFireGivenZombie(zombie);
		}
		//aqui iria animación de ataque si hay, poner el temporizador el tiempo de la animación
		GetWorld()->GetTimerManager().SetTimer(WaitTimer, this, &UWDiceSpreadCombat::SelectObjetiveSpreadFire, 3, false);
		UE_LOG(LogTemp, Warning, TEXT("aqui iria la animacion ataque cuerpo a cuerpo/normalFire"));
	}
}

void UWDiceSpreadCombat::SelectObjetiveSpreadFire()
{
	if (numberDiceLeft > 0) {
		if (SelectObjectiveWidgetClass) {
			if (SelectObjectiveWidget != nullptr) {
				SelectObjectiveWidget->SetNumberDiceLeft(numberDiceLeft);
				SelectObjectiveWidget->SetVisibility(ESlateVisibility::Visible);
			}
			else {
				SelectObjectiveWidget = CreateWidget<UWSelectObjectiveSpreadFire>(GetWorld(), SelectObjectiveWidgetClass);
				if (SelectObjectiveWidget != nullptr) {
					SelectObjectiveWidget->SetNumberDiceLeft(numberDiceLeft);
					SelectObjectiveWidget->AddToViewport();
				}
			}
		}
		APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (player != nullptr) {
			AGrid* grid = character->grid;
			UActionNewObjectiveSpreadFire* command = NewObject<UActionNewObjectiveSpreadFire>(this);
			command->tileZombie = tileZombie;
			command->NumberDiceLeft = numberDiceLeft;
			command->Execute(grid, character);
			UCommand* commandActions = NewObject<UActionNewObjectiveSpreadFire>(player->actions);
			player->actions->command = commandActions;
			player->actions->actionTile = grid->GetTileIndexFromLocation(character->GetActorLocation());
		}
	}
	else {
		EndAttack();
	}
}

void UWDiceSpreadCombat::EndAttack()
{
	DieNumber->SetVisibility(ESlateVisibility::Hidden);
	ButtonRollAnimation->SetVisibility(ESlateVisibility::Visible);
	ButtonConfirm->SetVisibility(ESlateVisibility::Hidden);
	if (hud != nullptr && hud->PlayerInfoWidget != nullptr) {
		hud->PlayerInfoWidget->UnhidePlayerInfoDuringCombat();
	}
	if (SelectObjectiveWidget != nullptr) {
		SelectObjectiveWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWDiceSpreadCombat::RemoveDie()
{
	if (hud != nullptr && hud->PlayerInfoWidget != nullptr) {
		hud->PlayerInfoWidget->SetZombieTileHovered(zombie);
	}
	currentNumber = humanDicesLeft[0];
	humanDicesLeft.RemoveSingle(currentNumber);
	numberDiceLeft = humanDicesLeft.Num();
	GetWorld()->GetTimerManager().SetTimer(WaitTimer, this, &UWDiceSpreadCombat::SelectObjetiveSpreadFire, 0.2, false);
}

void UWDiceSpreadCombat::OnClickButtonConfirm()
{
	SetVisibility(ESlateVisibility::Hidden);
	AttackSpreadFire();
}
