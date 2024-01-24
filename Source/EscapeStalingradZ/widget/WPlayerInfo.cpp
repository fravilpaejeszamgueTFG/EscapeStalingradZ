// Fill out your copyright notice in the Description page of Project Settings.

#include "WPlayerInfo.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Overlay.h"
#include "WActions.h"
#include "WObjectives.h"
#include "WOptions.h"

UWPlayerInfo::UWPlayerInfo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWPlayerInfo::NativeConstruct()
{

	Super::NativeConstruct();

	HealthBar->PercentDelegate.BindDynamic(this, &UWPlayerInfo::SetHealthBar);
	HealthBar->SynchronizeProperties();
	MPBar->PercentDelegate.BindDynamic(this, &UWPlayerInfo::SetMPBar);
	MPBar->SynchronizeProperties();
	health->TextDelegate.BindDynamic(this, &UWPlayerInfo::SetHealthText);
	health->SynchronizeProperties();
	MP->TextDelegate.BindDynamic(this, &UWPlayerInfo::SetMPText);
	MP->SynchronizeProperties();
	SecondaryWeapon_Overlay->VisibilityDelegate.BindDynamic(this, &UWPlayerInfo::SetVisibilitySecondaryWeapon);
	SecondaryWeapon_Overlay->SynchronizeProperties();
	//TO-DO setear los demas textos y cambiar seteo de vida y overlay para que no ocurra siempre
	
}

float UWPlayerInfo::SetHealthBar()
{
	if (character != nullptr) {
		return character->health / 4.f;
	}
	return 0;
}

float UWPlayerInfo::SetMPBar()
{
	if (character != nullptr) {
		if (character->typeOfMovement == Running) {
			return character->mp / 8.f;
		}
		return character->mp / 4.f;
	}
	return 0;
}

FText UWPlayerInfo::SetHealthText()
{
	if (character != nullptr) {
		FString healthText = FString::FromInt(character->health);
		healthText.Append(FString("/4"));
		return FText::FromString(healthText);
	}
	return FText();
}

FText UWPlayerInfo::SetMPText()
{
	if (character != nullptr) {
		FString mpText = FString::FromInt(character->mp); 
		if (character->typeOfMovement == Running) {
			mpText.Append(FString("/8"));
		}
		else {
			mpText.Append(FString("/4"));
		}
		return FText::FromString(mpText);
	}
	return FText();
}

FText UWPlayerInfo::SetKillNumberText()
{
	if (character != nullptr) {
		return FText::FromString(FString("x").Append(FString::FromInt(character->exp)));
	}
	return FText();
}

FText UWPlayerInfo::SetAmmoNumberText()
{
	if (character != nullptr) {
		return FText::FromString(FString("x").Append(FString::FromInt(character->ammo)));
	}
	return FText();
}

FText UWPlayerInfo::SetFoodNumberText()
{
	if (character != nullptr) {
		return FText::FromString(FString("x").Append(FString::FromInt(character->food)));
	}
	return FText();
}

FText UWPlayerInfo::SetMedKitNumberText()
{
	if (character != nullptr) {
		return FText::FromString(FString("x").Append(FString::FromInt(character->medkit)));
	}
	return FText();
}

ESlateVisibility UWPlayerInfo::SetVisibilitySecondaryWeapon()
{
	if (character != nullptr && character->readySecondaryWeapon->weaponName != EWeapon::None) {
		return ESlateVisibility::SelfHitTestInvisible;
	}
	return ESlateVisibility::Hidden;
}

void UWPlayerInfo::UpdateImages()
{
	SetCharacterImage();
	SetReadyWImage();
	SetSecondaryWImage();
}

void UWPlayerInfo::SetVisibleActionWidget()
{
	if (objectiveWidget != nullptr) {
		actionWidget->character = character;
		actionWidget->DisableButtonByMovementType(character->typeOfMovement);
		actionWidget->turn = turn;
		actionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UWPlayerInfo::SetVisibleOptionsWidget()
{
	if (optionsWidget != nullptr) {
		optionsWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UWPlayerInfo::SetVisibleObjectivesWidget()
{
	if (objectiveWidget != nullptr) {
		objectiveWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
