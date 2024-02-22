// Fill out your copyright notice in the Description page of Project Settings.


#include "UserHUD.h"
#include "WPlayerInfo.h"
#include "WReleaseLock.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

AUserHUD::AUserHUD() 
{

}

void AUserHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AUserHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AUserHUD::StartCharacterTurn()
{
	if (character != nullptr) {
		if (character->isLocked) {
			CreateOrSetReleaseLock();
		}
		else {
			CreateOrSetPlayerInfo();
		}
	}
}

void AUserHUD::CreateOrSetPlayerInfo()
{
	if (PlayerInfoWidgetClass) {
		if (PlayerInfoWidget != nullptr) {
			PlayerInfoWidget->character = character;
			PlayerInfoWidget->turn = turn;
			PlayerInfoWidget->UpdateImages();
			PlayerInfoWidget->SetVisibility(ESlateVisibility::Visible);
			PlayerInfoWidget->SetVisibleActionWidget();
		}
		else {
			PlayerInfoWidget = CreateWidget<UWPlayerInfo>(GetWorld(), PlayerInfoWidgetClass);
			if (PlayerInfoWidget != nullptr) {
				PlayerInfoWidget->character = character;
				PlayerInfoWidget->turn = turn;
				PlayerInfoWidget->UpdateImages();
				PlayerInfoWidget->AddToViewport();
				PlayerInfoWidget->SetVisibleActionWidget();
			}
		}
	}
}

void AUserHUD::CreateOrSetReleaseLock()
{
	if (ReleaseLockWidgetClass) {
		if (ReleaseLockWidget != nullptr) {
			ReleaseLockWidget->turn = turn;
			ReleaseLockWidget->character = character;
			ReleaseLockWidget->SetDicesNumbers();
			ReleaseLockWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			ReleaseLockWidget = CreateWidget<UWReleaseLock>(GetWorld(), ReleaseLockWidgetClass);
			if (ReleaseLockWidget != nullptr) {
				ReleaseLockWidget->turn = turn;
				ReleaseLockWidget->character = character;
				ReleaseLockWidget->SetDicesNumbers();
				ReleaseLockWidget->AddToViewport();
			}
		}
	}
}

void AUserHUD::HidePlayerInfo()
{
	if (PlayerInfoWidget != nullptr) {
		PlayerInfoWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
