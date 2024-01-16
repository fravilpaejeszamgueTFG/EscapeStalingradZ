// Fill out your copyright notice in the Description page of Project Settings.


#include "Turn.h"
#include "EscapeStalingradZ/grid/Grid.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/widget/WSelectCharacterTurn.h"
#include "EscapeStalingradZ/player/PlayerActions.h"
#include "Kismet/GameplayStatics.h"
#include "EscapeStalingradZ/player/PlayerC.h"
#include "EscapeStalingradZ/widget/WMovimiento.h"

// Sets default values
ATurn::ATurn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATurn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATurn::SetCharacters(TArray<FIntPoint> charactersList)
{
	if (grid != nullptr) {
		for (FIntPoint index : charactersList) {
			APlayerCharacter* chara = Cast<APlayerCharacter>(grid->gridTiles[index].actor);
			if (chara != nullptr) {
				characters.Add(chara);
			}
		}
		charactersToStartTurn = characters;
		SetNextCharacter();
	}
}

void ATurn::nextCharacter()
{
	if (selectedCharacter != nullptr) {
		APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (player != nullptr) {
			player->HideMovementWidget();
		}
		charactersToStartTurn.Remove(selectedCharacter);
		SetNextCharacter();
	}
}

void ATurn::SetNextCharacter()
{
	if (charactersToStartTurn.Num() > 1) {
		CreateSelectCharacterTurnWidget();
	}
	else if (charactersToStartTurn.Num() == 1) {
		selectedCharacter = charactersToStartTurn[0];
		APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (player != nullptr && player->actions != nullptr) {
			FIntPoint tile = grid->GetTileIndexFromLocation(selectedCharacter->GetActorLocation());
			player->actions->actionTile = tile;
			player->playerchara = selectedCharacter;
			player->SetMovementWidget();
			grid->AddTileState(tile, TileState::Selected);
			player->Movement->CreateEndTurnWidget(this);
		}
	}
	else {
		charactersToStartTurn = characters;
		endTurnHuman = true;
		UE_LOG(LogTemp, Warning, TEXT("Turno humano terminado"));
	}
}

void ATurn::CreateSelectCharacterTurnWidget()
{
	if (turnWidgetClass) {
		if (turnWidget != nullptr) {
			turnWidget->characters = charactersToStartTurn;
			turnWidget->SetVisibility(ESlateVisibility::Visible);			
			turnWidget->SetCharactersButton(this);
		}
		else {
			turnWidget = CreateWidget<UWSelectCharacterTurn>(GetWorld(), turnWidgetClass);
			if (turnWidget != nullptr) {
				turnWidget->characters = characters;
				turnWidget->AddToViewport();
				turnWidget->SetCharactersButton(this);
			}
		}
	}
}

