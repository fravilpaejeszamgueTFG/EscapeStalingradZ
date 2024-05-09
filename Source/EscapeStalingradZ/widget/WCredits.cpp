// Fill out your copyright notice in the Description page of Project Settings.


#include "WCredits.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

UWCredits::UWCredits(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWCredits::NativeConstruct()
{
	Super::NativeConstruct();

	buttonMainMenu->OnClicked.AddDynamic(this, &UWCredits::OnClickMainMenu);
}

void UWCredits::OnClickMainMenu()
{
	UGameplayStatics::OpenLevel(this, "MainMenu", true);
}

