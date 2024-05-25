// Fill out your copyright notice in the Description page of Project Settings.


#include "WOptions.h"
#include "Kismet/GameplayStatics.h"
#include "UserHUD.h"
#include "Components/Button.h"
#include "WOptionsMenu.h"


UWOptions::UWOptions(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWOptions::NativeConstruct()
{

	Super::NativeConstruct();

	hud = Cast<AUserHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	Inventory->OnClicked.AddDynamic(this, &UWOptions::OnClickInventory);
	Options->OnClicked.AddDynamic(this, &UWOptions::OnClickOptions);
}

void UWOptions::OnClickInventory()
{
	if (hud != nullptr) {
		hud->CreateOrSetInventory();
	}
}

void UWOptions::OnClickOptions()
{
	if (OptionMenuWidgetClass) {
		if (OptionMenuWidget != nullptr) {
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			OptionMenuWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			OptionMenuWidget = CreateWidget<UWOptionsMenu>(GetWorld(), OptionMenuWidgetClass);
			if (OptionMenuWidget != nullptr) {
				UGameplayStatics::SetGamePaused(GetWorld(), true);
				OptionMenuWidget->AddToViewport();
			}
		}
	}
}
