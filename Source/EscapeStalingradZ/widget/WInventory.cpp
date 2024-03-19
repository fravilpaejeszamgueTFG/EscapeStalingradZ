// Fill out your copyright notice in the Description page of Project Settings.


#include "WInventory.h"
#include "Kismet/GameplayStatics.h"
#include "UserHUD.h"
#include "Components/Button.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "Components/TextBlock.h"

UWInventory::UWInventory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWInventory::NativeConstruct()
{
	Super::NativeConstruct();

	Back->OnClicked.AddDynamic(this, &UWInventory::OnClickGoBack);

}

void UWInventory::OnClickSlot1()
{
	
}

void UWInventory::OnClickSlot2()
{

}

void UWInventory::OnClickSlot3()
{

}

void UWInventory::OnClickSlot4()
{

}

void UWInventory::OnClickSlotReadyWeapon()
{

}

void UWInventory::OnClickSlotSecondaryWeapon()
{

}

void UWInventory::OnClickGoBack()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UWInventory::SetTextNumbers()
{
	if (character != nullptr) {
		AmmoNumber->SetText(FText::FromString(FString("x").Append(FString::FromInt(character->ammo))));
		FoodNumber->SetText(FText::FromString(FString("x").Append(FString::FromInt(character->food))));
		MedKitNumber->SetText(FText::FromString(FString("x").Append(FString::FromInt(character->medkit))));
	}
	
}
