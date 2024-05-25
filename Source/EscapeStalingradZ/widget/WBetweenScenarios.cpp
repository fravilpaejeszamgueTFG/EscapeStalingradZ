// Fill out your copyright notice in the Description page of Project Settings.


#include "WBetweenScenarios.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "UserHUD.h"
#include "WInventory.h"
#include "TimerManager.h"


UWBetweenScenarios::UWBetweenScenarios(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWBetweenScenarios::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonLeft->OnClicked.AddDynamic(this, &UWBetweenScenarios::OnClickButtonLeft);
	ButtonRight->OnClicked.AddDynamic(this, &UWBetweenScenarios::OnClickButtonRight);
	useFood->OnClicked.AddDynamic(this, &UWBetweenScenarios::OnClickButtonFood);
	useMedKit->OnClicked.AddDynamic(this, &UWBetweenScenarios::OnClickButtonMedKit);
	inventory->OnClicked.AddDynamic(this, &UWBetweenScenarios::OnClickButtonInventory);
	changeEquipment->OnClicked.AddDynamic(this, &UWBetweenScenarios::OnClickButtonChangeEquipment);
	confirm->OnClicked.AddDynamic(this, &UWBetweenScenarios::OnClickButtonConfirm);

	SetInitialValues();

	GetWorld()->GetTimerManager().SetTimer(timer, this, &UWBetweenScenarios::EnableButtons, 0.25, false); //una vez tenemos todos los personajes se habilitan los botones

}

void UWBetweenScenarios::OnClickButtonLeft()
{
	if (index <= 0) {
		index = characters.Num() - 1;
	}
	else {
		index--;
	}
	currentCharacter = characters[index];
	currentCharacterChosen = currentCharacter->characterChosen;
	SetImages(false);
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UWBetweenScenarios::SetHealthAfterChanginCharacter, 1.f, false);
}

void UWBetweenScenarios::OnClickButtonRight()
{
	if (index >= characters.Num() - 1) {
		index = 0;
	}
	else {
		index++;
	}
	currentCharacter = characters[index];
	currentCharacterChosen = currentCharacter->characterChosen;
	SetImages(true);
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UWBetweenScenarios::SetHealthAfterChanginCharacter, 1.f, false);
}

void UWBetweenScenarios::OnClickButtonConfirm()
{
	SetVisibility(ESlateVisibility::Hidden);
	RemoveFromParent();
	if (hud != nullptr) {
		if (GetWorld()->GetName() == "AFriendWillBleed") {
			hud->CreateSelectCharacterToFreeWidget(currentCharacter);
		}
		else {
			hud->StartGameAfterSelectCharacter();
		}
	}
	Destruct();
}

void UWBetweenScenarios::OnClickButtonFood()
{
	if (currentCharacter != nullptr && currentCharacter->health < 4) {
		currentCharacter->health++;
		currentCharacter->food--;
		EnableButtonFood();
		EnableButtonMedKit();
		SetFoodText();
		SetHealthText();
		SetHealthBar();
	}
}

void UWBetweenScenarios::OnClickButtonMedKit()
{
	if (currentCharacter != nullptr && currentCharacter->health < 3) {
		currentCharacter->health += 2;
		currentCharacter->medkit--;
		EnableButtonFood();
		EnableButtonMedKit();
		SetMedKitText();
		SetHealthText();
		SetHealthBar();
	}
}

void UWBetweenScenarios::OnClickButtonInventory()
{
	if (InventoryWidgetClass) {
		if (InventoryWidget != nullptr) {
			InventoryWidget->inBetweenScenarios = true;
			InventoryWidget->character = currentCharacter;
			InventoryWidget->SetTextNumbers();
			InventoryWidget->SetImages();
			InventoryWidget->ResetSelectedWeapon();
			InventoryWidget->DisableButtons();
			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			InventoryWidget = CreateWidget<UWInventory>(GetWorld(), InventoryWidgetClass);
			if (InventoryWidget != nullptr) {
				InventoryWidget->inBetweenScenarios = true;
				InventoryWidget->character = currentCharacter;
				InventoryWidget->hud = hud;
				InventoryWidget->SetTextNumbers();
				InventoryWidget->SetImages();
				InventoryWidget->ResetSelectedWeapon();
				InventoryWidget->DisableButtons();
				InventoryWidget->AddToViewport();
			}
		}
	}
}

void UWBetweenScenarios::OnClickButtonChangeEquipment()
{
	if (hud != nullptr) {
		hud->CreateOrSetExchangeEquipment(currentCharacter, characters[(index + 1) % 2]);
	}
}

void UWBetweenScenarios::SetImages(bool moveRight)
{
	DisableButtons();
	SetImagesBP(moveRight);
}

void UWBetweenScenarios::DisableButtons()
{
	confirm->SetIsEnabled(false);
	ButtonLeft->SetIsEnabled(false);
	ButtonRight->SetIsEnabled(false);
	useMedKit->SetIsEnabled(false);
	useFood->SetIsEnabled(false);
	inventory->SetIsEnabled(false);
	changeEquipment->SetIsEnabled(false);
}

void UWBetweenScenarios::EnableButtons()
{
	confirm->SetIsEnabled(true);
	inventory->SetIsEnabled(true);
	if (characters.Num() > 1) { // hay mas de un personaje
		ButtonLeft->SetIsEnabled(true);
		ButtonRight->SetIsEnabled(true);
		changeEquipment->SetIsEnabled(true);
	}
	EnableButtonFood();
	EnableButtonMedKit();
}

void UWBetweenScenarios::SetHealthBar()
{
	if (currentCharacter != nullptr) {
		HealthBar->SetPercent(currentCharacter->health / 4.f);
	}
}

void UWBetweenScenarios::SetHealthText()
{
	if (currentCharacter != nullptr) {
		health->SetText(FText::FromString(FString::FromInt(currentCharacter->health).Append(FString("/4"))));
	}
}

void UWBetweenScenarios::SetAmmoText()
{
	if (currentCharacter != nullptr) {
		AmmoNumber->SetText(FText::FromString(FString("x").Append(FString::FromInt(currentCharacter->ammo))));
	}
}

void UWBetweenScenarios::SetFoodText()
{
	if (currentCharacter != nullptr) {
		FoodNumber->SetText(FText::FromString(FString("x").Append(FString::FromInt(currentCharacter->food))));
	}
}

void UWBetweenScenarios::SetMedKitText()
{
	if (currentCharacter != nullptr) {
		MedKitNumber->SetText(FText::FromString(FString("x").Append(FString::FromInt(currentCharacter->medkit))));
	}
}

void UWBetweenScenarios::SetInitialValues()
{
	SetHealthBar();
	SetHealthText();
	SetFoodText();
	SetMedKitText();
	SetAmmoText();
	ButtonLeft->SetIsEnabled(false);
	ButtonRight->SetIsEnabled(false);
	changeEquipment->SetIsEnabled(false);
}

void UWBetweenScenarios::EnableButtonFood()
{
	if (currentCharacter != nullptr && currentCharacter->health < 4 && currentCharacter->food > 0) {
		useFood->SetIsEnabled(true);
	}
	else {
		useFood->SetIsEnabled(false);
	}
}

void UWBetweenScenarios::EnableButtonMedKit()
{
	if (currentCharacter != nullptr && currentCharacter->health < 3 && currentCharacter->medkit > 0) {
		useMedKit->SetIsEnabled(true);
	}
	else {
		useMedKit->SetIsEnabled(false);
	}
}

void UWBetweenScenarios::SetHealthAfterChanginCharacter()
{
	SetHealthBar();
	SetHealthText();
	SetMedKitText();
	SetFoodText();
	SetAmmoText();
}
