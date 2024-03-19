// Fill out your copyright notice in the Description page of Project Settings.


#include "WOptions.h"
#include "Kismet/GameplayStatics.h"
#include "UserHUD.h"
#include "Components/Button.h"


UWOptions::UWOptions(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWOptions::NativeConstruct()
{

	Super::NativeConstruct();

	hud = Cast<AUserHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	Inventory->OnClicked.AddDynamic(this, &UWOptions::OnClickInventory);
}

void UWOptions::OnClickInventory()
{
	if (hud != nullptr) {
		hud->CreateOrSetInventory();
	}
}
