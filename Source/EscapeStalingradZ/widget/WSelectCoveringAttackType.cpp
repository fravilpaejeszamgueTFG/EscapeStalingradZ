// Fill out your copyright notice in the Description page of Project Settings.


#include "WSelectCoveringAttackType.h"
#include "Components/Button.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/turn/Turn.h"
#include "Kismet/GameplayStatics.h"
#include "EscapeStalingradZ/widget/UserHUD.h"

UWSelectCoveringAttackType::UWSelectCoveringAttackType(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWSelectCoveringAttackType::NativeConstruct()
{
	Super::NativeConstruct();

	buttonHandToHand->OnClicked.AddDynamic(this, &UWSelectCoveringAttackType::OnClickHandToHand);
	buttonNormalFire->OnClicked.AddDynamic(this, &UWSelectCoveringAttackType::OnClickNormalFire);
	buttonSpreadFire->OnClicked.AddDynamic(this, &UWSelectCoveringAttackType::OnClickSpreadFire);

	hud = Cast<AUserHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void UWSelectCoveringAttackType::OnClickHandToHand()
{
	typeOfAttack = CoveringAttackType::HandToHand;
	OnClickConfirm();
}

void UWSelectCoveringAttackType::OnClickNormalFire()
{
	typeOfAttack = CoveringAttackType::NormalFire;
	OnClickConfirm();
}

void UWSelectCoveringAttackType::OnClickSpreadFire()
{
	typeOfAttack = CoveringAttackType::SpreadFire;
	OnClickConfirm();
}

void UWSelectCoveringAttackType::OnClickConfirm()
{
	if (character != nullptr && turn != nullptr) {
		character->typeOfCoveringAttack = typeOfAttack;
		if (character->typeOfMovement == MovementType::Running) {
			character->typeOfCovering = CoveringType::Ran;
		}
		else if (character->typeOfMovement == MovementType::Walking) {
			character->typeOfCovering = CoveringType::Walked;
		}
		else {
			character->typeOfCovering = CoveringType::CStationary;
		}
		turn->nextCharacter();
		hud->HidePlayerInfo();
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWSelectCoveringAttackType::UpdateButtons()
{
	if (character != nullptr) {
		typeOfAttack = CoveringAttackType::HandToHand;
		if (character->ammo <= 0) {
			buttonNormalFire->SetVisibility(ESlateVisibility::Hidden);
			buttonSpreadFire->SetVisibility(ESlateVisibility::Hidden);
		}
		else {
			if (character->useReadyWeapon) {
				if (character->readyWeapon->multipleRange > 0) {
					buttonNormalFire->SetVisibility(ESlateVisibility::Visible);
					if (character->readyWeapon->rateOfFire > 1) {
						buttonSpreadFire->SetVisibility(ESlateVisibility::Visible);
					}
					else {
						buttonSpreadFire->SetVisibility(ESlateVisibility::Hidden);
					}
				}
				else {
					buttonNormalFire->SetVisibility(ESlateVisibility::Hidden);
					buttonSpreadFire->SetVisibility(ESlateVisibility::Hidden);
				}
			}
			else {
				if (character->readySecondaryWeapon->multipleRange > 0) {
					buttonNormalFire->SetVisibility(ESlateVisibility::Visible);
					if (character->readySecondaryWeapon->rateOfFire > 1) {
						buttonSpreadFire->SetVisibility(ESlateVisibility::Visible);
					}
					else {
						buttonSpreadFire->SetVisibility(ESlateVisibility::Hidden);
					}
				}
				else {
					buttonNormalFire->SetVisibility(ESlateVisibility::Hidden);
					buttonSpreadFire->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
	}
}
