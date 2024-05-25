// Fill out your copyright notice in the Description page of Project Settings.


#include "WZombieInfo.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "EscapeStalingradZ/zombies/Zombie.h"

UWZombieInfo::UWZombieInfo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWZombieInfo::NativeConstruct()
{
	Super::NativeConstruct();

	healthBar->PercentDelegate.BindDynamic(this, &UWZombieInfo::SetHealthBar);
	healthBar->SynchronizeProperties();
	health->TextDelegate.BindDynamic(this, &UWZombieInfo::SetHealthText);
	health->SynchronizeProperties();
	stunImage->VisibilityDelegate.BindDynamic(this, &UWZombieInfo::SetVisibilityStunImage);
	stunImage->SynchronizeProperties();

}

float UWZombieInfo::SetHealthBar()
{
	if (zombie != nullptr) {
		if (zombie->typeOfZombie == ZombieType::Alpha) {
			return zombie->health / 2.f;
		}
		else {
			return zombie->health / 1.f;
		}
	}
	return 0;
}

FText UWZombieInfo::SetHealthText()
{
	if (zombie != nullptr) {
		FString healthText = FString::FromInt(zombie->health);
		if (zombie->typeOfZombie == ZombieType::Alpha) {
			healthText.Append(FString("/2"));
		}
		else {
			healthText.Append(FString("/1"));
		}
		return FText::FromString(healthText);
	}
	return FText();
}

ESlateVisibility UWZombieInfo::SetVisibilityStunImage()
{
	if (zombie != nullptr && zombie->isStunned) {
		return ESlateVisibility::SelfHitTestInvisible;
	}
	return ESlateVisibility::Hidden;
}
