// Fill out your copyright notice in the Description page of Project Settings.


#include "UserHUD.h"
#include "WPlayerInfo.h"
#include "WReleaseLock.h"
#include "WInventory.h"
#include "WSelectCharacter.h"
#include "WSelectCharacterToFree.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/character/CharacterToFree.h"
#include "EscapeStalingradZ/turn/Turn.h"
#include "Kismet/GameplayStatics.h"
#include "WBetweenScenarios.h"
#include "WExchangeEquipment.h"
#include "WDeathScreen.h"
#include "WSettings.h"
#include "WCredits.h"

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

void AUserHUD::CreateOrSetInventory()
{
	if (InventoryWidgetClass) {
		if (InventoryWidget != nullptr) {
			InventoryWidget->inBetweenScenarios = false;
			InventoryWidget->character = character;
			InventoryWidget->SetTextNumbers();
			InventoryWidget->SetImages();
			InventoryWidget->ResetSelectedWeapon();
			InventoryWidget->DisableButtons();
			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			InventoryWidget = CreateWidget<UWInventory>(GetWorld(), InventoryWidgetClass);
			if (InventoryWidget != nullptr) {
				InventoryWidget->inBetweenScenarios = false;
				InventoryWidget->character = character;
				InventoryWidget->hud = this;
				InventoryWidget->SetTextNumbers();
				InventoryWidget->SetImages();
				InventoryWidget->ResetSelectedWeapon();
				InventoryWidget->DisableButtons();
				InventoryWidget->AddToViewport();
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

void AUserHUD::StartGameAfterSelectCharacter()
{
	if (turn != nullptr) {
		turn->StartTurn();
	}
}

void AUserHUD::CreateSelectCharacterWidget(APlayerCharacter* chara)
{
	if (SelectCharacterWidgetClass) {
		UWSelectCharacter* selectCharacter = CreateWidget<UWSelectCharacter>(GetWorld(), SelectCharacterWidgetClass);
		if (selectCharacter != nullptr) {
			selectCharacter->character = chara;
			selectCharacter->hud = this;
			selectCharacter->AddToViewport();
		}
	}
}

void AUserHUD::CreateSelectCharacterToFreeWidget(APlayerCharacter* chara)
{
	if (SelectCharacterToFreeWidgetClass && characterToFree != nullptr) {
		UWSelectCharacterToFree* selectCharacter = CreateWidget<UWSelectCharacterToFree>(GetWorld(), SelectCharacterToFreeWidgetClass);
		if (selectCharacter != nullptr) {
			selectCharacter->SetCharactersAvailable(chara->characterChosen);
			selectCharacter->character = characterToFree;
			selectCharacter->hud = this;
			selectCharacter->AddToViewport();
		}
	}
}

void AUserHUD::SetPrimaryCharacterToBetweenScenarios(APlayerCharacter* chara)
{
	if (BetweenScenariosWidgetClass) {
		betweenScenariosWidget = CreateWidget<UWBetweenScenarios>(GetWorld(), BetweenScenariosWidgetClass);
		if (betweenScenariosWidget != nullptr) {
			betweenScenariosWidget->currentCharacter = chara;
			betweenScenariosWidget->currentCharacterChosen = chara->characterChosen;
			betweenScenariosWidget->characters.Add(chara);
			betweenScenariosWidget->hud = this;
			betweenScenariosWidget->SetInitialImage();
			betweenScenariosWidget->AddToViewport();
		}
	}
}

void AUserHUD::SetSecondaryCharacterToBetweenScenarios(APlayerCharacter* chara)
{
	if (betweenScenariosWidget != nullptr) {
		betweenScenariosWidget->characters.Add(chara);
	}
}

void AUserHUD::CreateOrSetExchangeEquipment(APlayerCharacter* character1, APlayerCharacter* character2)
{
	if (ExchangeEquipmentWidgetClass) {
		if (ExchangeEquipmentWidget != nullptr) {
			ExchangeEquipmentWidget->character1 = character1;
			ExchangeEquipmentWidget->character2 = character2;
			ExchangeEquipmentWidget->SetTextNumbers();
			ExchangeEquipmentWidget->SetImages();
			ExchangeEquipmentWidget->ResetSelectedCharacter();
			ExchangeEquipmentWidget->DisableButtons();
			ExchangeEquipmentWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			ExchangeEquipmentWidget = CreateWidget<UWExchangeEquipment>(GetWorld(), ExchangeEquipmentWidgetClass);
			if (ExchangeEquipmentWidget != nullptr) {
				ExchangeEquipmentWidget->betweenWidget = betweenScenariosWidget;
				ExchangeEquipmentWidget->character1 = character1;
				ExchangeEquipmentWidget->character2 = character2;
				ExchangeEquipmentWidget->SetTextNumbers();
				ExchangeEquipmentWidget->SetImages();
				ExchangeEquipmentWidget->ResetSelectedCharacter();
				ExchangeEquipmentWidget->DisableButtons();
				ExchangeEquipmentWidget->SetVisibility(ESlateVisibility::Visible);
				ExchangeEquipmentWidget->AddToViewport();
			}
		}
	}
}

void AUserHUD::EndGame()
{
	if (DeathScreenWidgetClass) {
		UWDeathScreen* deathWidget = CreateWidget<UWDeathScreen>(GetWorld(), DeathScreenWidgetClass);
		if (deathWidget != nullptr) {
			deathWidget->AddToViewport();
			deathWidget->StartAnimation();
		}
	}
}

void AUserHUD::CreateOrSetSelectSettingsWidget()
{
	if (SettingsWidgetClass) {
		if (SettingsWidget != nullptr) {
			SettingsWidget->SetVisibility(ESlateVisibility::Visible);
			SettingsWidget->DisableButtonsInitial();
		}
		else {
			SettingsWidget = CreateWidget<UWSettings>(GetWorld(), SettingsWidgetClass);
			if (SettingsWidget != nullptr) {
				SettingsWidget->AddToViewport();
				SettingsWidget->DisableButtonsInitial();
			}
		}
	}
}

void AUserHUD::FinishGame()
{
	if (CreditWidgetClass) {
		UWCredits* creditWidget = CreateWidget<UWCredits>(GetWorld(), CreditWidgetClass);
		if (creditWidget != nullptr) {
			creditWidget->AddToViewport();
		}
	}
}
