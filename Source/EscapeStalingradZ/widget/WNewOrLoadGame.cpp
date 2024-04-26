// Fill out your copyright notice in the Description page of Project Settings.


#include "WNewOrLoadGame.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "EscapeStalingradZ/instances/GI.h"
#include "Kismet/GameplayStatics.h"

UWNewOrLoadGame::UWNewOrLoadGame(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWNewOrLoadGame::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonSlot1->OnClicked.AddDynamic(this, &UWNewOrLoadGame::OnClickButtonSlot1);
	ButtonSlot2->OnClicked.AddDynamic(this, &UWNewOrLoadGame::OnClickButtonSlot2);
	ButtonSlot3->OnClicked.AddDynamic(this, &UWNewOrLoadGame::OnClickButtonSlot3);

	GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void UWNewOrLoadGame::OnClickButtonSlot1()
{
	if (GI != nullptr) {
		if (isNewGame) {
			//TO-DO comprobar que no haya guardado en ese slot, si lo hay boton de confirmacion para sobreescribir
			GI->currentSlot = FString("Slot1");
			GI->LoadGame();
			UGameplayStatics::OpenLevel(this, "FUBAR", true);
		}
	}
}

void UWNewOrLoadGame::OnClickButtonSlot2()
{
	if (GI != nullptr) {
		if (isNewGame) {
			GI->currentSlot = FString("Slot2");
			UGameplayStatics::OpenLevel(this, "FUBAR", true);
		}
	}
}

void UWNewOrLoadGame::OnClickButtonSlot3()
{
	if (GI != nullptr) {
		if (isNewGame) {
			GI->currentSlot = FString("Slot3");
			UGameplayStatics::OpenLevel(this, "FUBAR", true);
		}
	}
}

void UWNewOrLoadGame::SetVisibilityTextAndImages()
{
	//TO-DO cambiar cuando este hecho el guardado en la carga de partida
	HorizontalBoxSaveGameSlot1->SetVisibility(ESlateVisibility::Hidden);
	HorizontalBoxSaveGameSlot2->SetVisibility(ESlateVisibility::Hidden);
	HorizontalBoxSaveGameSlot3->SetVisibility(ESlateVisibility::Hidden);
}

void UWNewOrLoadGame::PrecacheGivenPackage(const FString& name)
{
	// Using LoadPackageAsync to precache the level
	LoadPackageAsync(name,
		FLoadPackageAsyncDelegate::CreateLambda([=](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
			{
				if (Result == EAsyncLoadingResult::Succeeded)
				{
					packages.Add(LoadedPackage);
				}
			}),
		0,
				PKG_ContainsMap);
}
