// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerC.h"
#include "Components/WidgetComponent.h"
#include "EscapeStalingradZ/widget/WSelectMovementType.h"
#include "EscapeStalingradZ/instances/GI.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerActions.h"
#include "UObject/UObjectGlobals.h"


APlayerC::APlayerC()
{
	actions = CreateDefaultSubobject<UPlayerActions>("actions");

	SetActorHiddenInGame(false);
}

// Called when the game starts or when spawned
void APlayerC::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	EnableInput(this);

	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &APlayerC::LeftMouseClick);

	GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr)
	{
		if (GetWorld()->GetName() == "FUBAR") {
			GI->currentLevel = ScenarioName::FUBAR;
			PrecacheGivenPackage("/Game/Maps/WakeUp");
		}
		else if (GetWorld()->GetName() == "WakeUp") {
			GI->currentLevel = ScenarioName::WAKEUP;
			canExitTheRoom = false;
			//PrecacheGivenPackage("/Game/Maps/WakeUp");
			//PrecacheGivenPackage("/Game/Maps/WakeUp");
		}
		else if (GetWorld()->GetName() == "AFriendWillBleed") {
			GI->currentLevel = ScenarioName::AFRIEND;
			canExitTheRoom = false;
		}
		else if (GetWorld()->GetName() == "STASH") {
			GI->currentLevel = ScenarioName::STASH;
		}
		else if (GetWorld()->GetName() == "MoveAlong") {
			GI->currentLevel = ScenarioName::MOVEALONG;
		}
	}
}

void APlayerC::LeftMouseClick()
{
	actions->LeftMouseClick();
}

void APlayerC::SetMovementWidget()
{
	if (MovementClass) {
		if (Movement != nullptr) {
			Movement->character = playerchara;
			Movement->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			Movement = CreateWidget<UWSelectMovementType>(GetWorld(), MovementClass);
			if (Movement != nullptr) {
				Movement->character = playerchara;
				Movement->AddToViewport();
			}
		}
	}
}

void APlayerC::HideMovementWidget()
{
	Movement->SetVisibility(ESlateVisibility::Hidden);
}

void APlayerC::PrecacheGivenPackage(const FString& name)
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

void APlayerC::ChangeLevel(int exitNumber)
{
	if (GI != nullptr) {
		if (GetWorld()->GetName() == "FUBAR") {
			GI->levelsPlayed.Add(ScenarioName::FUBAR);
			UGameplayStatics::OpenLevel(this, "WakeUp", true);
		}
		else if (GetWorld()->GetName() == "WakeUp") {
			GI->levelsPlayed.Add(ScenarioName::WAKEUP);
			if (exitNumber > 0) {
				//UGameplayStatics::OpenLevel(this, "WakeUp", true); s2
			}
			else {
				//UGameplayStatics::OpenLevel(this, "WakeUp", true); s1
			}
		}
		else if (GetWorld()->GetName() == "AFriendWillBleed") {
			GI->levelsPlayed.Add(ScenarioName::AFRIEND);
		}
		else if (GetWorld()->GetName() == "STASH") {
			GI->levelsPlayed.Add(ScenarioName::STASH);
		}
		else if (GetWorld()->GetName() == "MoveAlong") {
			GI->levelsPlayed.Add(ScenarioName::MOVEALONG);
		}
	}
}
