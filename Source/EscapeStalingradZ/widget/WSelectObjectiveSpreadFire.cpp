// Fill out your copyright notice in the Description page of Project Settings.


#include "WSelectObjectiveSpreadFire.h"
#include "Components/TextBlock.h"

UWSelectObjectiveSpreadFire::UWSelectObjectiveSpreadFire(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWSelectObjectiveSpreadFire::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWSelectObjectiveSpreadFire::SetNumberDiceLeft(int number)
{
	NumberDiceLeftText->SetText(FText::FromString(FString::FromInt(number)));
}


