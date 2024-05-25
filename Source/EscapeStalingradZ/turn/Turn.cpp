// Fill out your copyright notice in the Description page of Project Settings.


#include "Turn.h"
#include "Components/BoxComponent.h"
#include "EscapeStalingradZ/Grid/Grid.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/widget/WSelectCharacterTurn.h"
#include "EscapeStalingradZ/player/PlayerActions.h"
#include "Kismet/GameplayStatics.h"
#include "EscapeStalingradZ/player/PlayerC.h"
#include "EscapeStalingradZ/widget/WSelectMovementType.h"
#include "EscapeStalingradZ/widget/UserHUD.h"
#include "EscapeStalingradZ/widget/WPlayerInfo.h"
#include "EscapeStalingradZ/widget/WDicesTurn.h"
#include "EscapeStalingradZ/zombies/Zombie.h"
#include "EscapeStalingradZ/player/actions/ActionSelectTileToMove.h"
#include "EscapeStalingradZ/widget/WObjectives.h"
#include "EscapeStalingradZ/misc/StunIcon.h"
#include "EscapeStalingradZ/instances/GI.h"

// Sets default values
ATurn::ATurn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(FName("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	boxCollision = CreateDefaultSubobject<UBoxComponent>("boxCollision");
	boxCollision->SetupAttachment(DefaultSceneRoot);

	SetActorEnableCollision(true);

}

// Called when the game starts or when spawned
void ATurn::BeginPlay()
{
	Super::BeginPlay();

	hud = Cast<AUserHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	hud->turn = this;

	UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		int numDeathCharacters = GI->numberOfDeathCharacters;
		for (int i = 0; i < numDeathCharacters; i++) {
			SpawnInitialZombieCharacters();
		}
	}

	GetWorldTimerManager().SetTimer(WaitTimer, this, &ATurn::ActivateCollision, 0.6, false);
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
	}
}

void ATurn::nextCharacter()
{
	grid->deleteStatesFromTiles();
	if (selectedCharacter != nullptr) {
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
		selectedCharacter->attacked = false;
		selectedCharacter->typeOfCovering = CoveringType::NONE;
		PrepareCharacterForTurn();
	}
	else {
		if (isFubar && roundNumber >= 6) {
			hud->EndGame();
		}
		else {
			hud->HidePlayerInfo();
			charactersToStartTurn = characters;
			endTurnHuman = true;
			if (endTurnZombie) {
				EndTurn();
			}
			else {
				zombiesToStartTurn = zombies;
				SetNextZombie();
			}
		}	
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

void ATurn::ActivateCollision()
{
	TArray<AActor*> z;
	GetOverlappingActors(z);
	for (AActor* i : z) {
		AZombie* zombie = Cast<AZombie>(i);
		if (zombie != nullptr) {
			zombie->turn = this;
			zombie->SetActorEnableCollision(false);
			zombiesToEnterGrid.Add(zombie);
		}
	}
	if (zombiesToEnterGrid.Num() > 0) {
		// Shuffle the zombies
		const int32 NumShuffles = zombiesToEnterGrid.Num() - 1;
		for (int32 i = 0; i < NumShuffles; ++i)
		{
			int32 SwapIdx = FMath::RandRange(i, NumShuffles);
			zombiesToEnterGrid.Swap(i, SwapIdx);
		}
	}
	SetActorEnableCollision(false);
	SpawnZombieBeforeStartGame();
}

void ATurn::nextZombie()
{
	grid->deleteStatesFromTiles();
	if (selectedZombie != nullptr) {
		selectedZombie->charactersInCovering.Empty();
		zombiesToStartTurn.Remove(selectedZombie);
		SetNextZombie();
	}
}

void ATurn::SetNextZombie()
{
	if (zombiesToStartTurn.Num() > 0) {
		selectedZombie = zombiesToStartTurn[0];
		selectedZombie->characters = characters;
		selectedZombie->ZombieActions();
	}
	else {
		endTurnZombie = true;
		selectedZombie = nullptr;
		RemoveStunToZombies();
		ResetZombiesWhenAllAreDead();
		SpawnZombies();
	}
}

void ATurn::SpawnZombies()
{
	if (spawnZombiesTiles.Num() > 0) {
		int numRound = roundNumber % spawnZombiesTiles.Num();
		FIntPoint tile = spawnZombiesTiles[numRound];
		if (tile != FIntPoint(-1, -1) && zombiesToEnterGrid.Num() > 0) {
			AZombie* newZombie = zombiesToEnterGrid[0];
			zombiesToEnterGrid.Remove(newZombie);
			newZombie->startIndex = tile;
			if (grid->gridTiles[tile].actor == nullptr) {
				grid->SetZombieStartLocation(newZombie);
				zombies.Add(newZombie);
				DirectContactInSpawnZombie(tile);
				EndZombieTurn();
			}
			else {
				AZombie* zombieInTile = Cast<AZombie>(grid->gridTiles[tile].actor);
				if (zombieInTile != nullptr) {
					if (zombiesWaitingToEnterGrid.Contains(tile)) {
						zombiesWaitingToEnterGrid[tile].zombies.Add(newZombie);
					}
					else {
						TArray<AZombie*> z = TArray<AZombie*>();
						z.Add(newZombie);
						zombiesWaitingToEnterGrid.Add(tile, FZombiesWaiting(z));
					}
					EndZombieTurn();
				}
				else {
					SpawnZombieInCharacterTile(newZombie, tile, false);
				}
			}
		}
		else {
			EndZombieTurn();
		}
	}
}

void ATurn::EndZombieTurn()
{
	if (endTurnHuman) {
		EndTurn();
	}
	else {
		charactersToStartTurn = characters;
		SetNextCharacter();
	}
}

void ATurn::StartTurn()
{
	Initiative();
}

void ATurn::ZombiesStartTurn()
{
	zombiesToStartTurn = zombies;
	SetNextZombie();
}

void ATurn::Initiative()
{
	charactersToStartTurn = characters;
	int humanDie = FMath::RandRange(1, 12);
	int zombieDie = FMath::RandRange(1, 12);
	int finalZombieDie = zombieDie - 4;
	bool alphaOnBoard = false;
	for (AZombie* z : zombies) {
		if (z->typeOfZombie == ZombieType::Alpha && z->health >= 2) {
			finalZombieDie += 4;
			alphaOnBoard = true;
			break;
		}
	}
	if (alphaOnBoard) {
		for (AZombie* z : zombies) {
			if (z->typeOfZombie == ZombieType::Beta || z->typeOfZombie == ZombieType::BetaCharacter) {
				z->maxmp = 2;
				z->mp = 2;
			}
		}
	}
	CreateOrSetTurnDicesWidget(humanDie, finalZombieDie);
}

void ATurn::EndTurn()
{
	endTurnZombie = false;
	endTurnHuman = false;
	roundNumber++;
	if (CurrentObjective != nullptr) {
		CurrentObjective->SetNumberRoundInImage(roundNumber);
	}
	for (APlayerCharacter* chara : characters) {
		grid->DamageIfCharacterInPoisonTile(chara);
	}
	StartTurn();
}

void ATurn::CreateOrSetTurnDicesWidget(int humanDie, int targetDie)
{
	if (turnDicesWidgetClass) {
		if (turnDicesWidget != nullptr) {
			turnDicesWidget->SetVisibility(ESlateVisibility::Visible);
			turnDicesWidget->SetDices(humanDie, targetDie);
			turnDicesWidget->turn = this;
		}
		else {
			turnDicesWidget = CreateWidget<UWDicesTurn>(GetWorld(), turnDicesWidgetClass);
			if (turnDicesWidget != nullptr) {
				turnDicesWidget->AddToViewport();
				turnDicesWidget->SetDices(humanDie, targetDie);
				turnDicesWidget->turn = this;
			}
		}
	}
}

void ATurn::RemoveStunToZombies()
{
	for (AZombie* z : zombies) {
		z->isStunned = false;
	}
}

void ATurn::PrepareCharacterForTurn()
{
	APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (player != nullptr && player->actions != nullptr) {
		FIntPoint tile = grid->GetTileIndexFromLocation(selectedCharacter->GetActorLocation());
		player->actions->actionTile = tile;
		player->playerchara = selectedCharacter;
		player->SetMovementWidget();
		grid->AddTileState(tile, TileState::Selected);
		player->Movement->turn = this;
	}
}

void ATurn::DirectContactInSpawnZombie(FIntPoint tile)
{
	TArray<FIntPoint> neighbors = grid->GetTileNeighbors(tile);
	for (FIntPoint index : neighbors) {
		if (grid->gridTiles[index].actor != nullptr) {
			APlayerCharacter* chara = Cast<APlayerCharacter>(grid->gridTiles[index].actor);
			if (chara != nullptr && !chara->inDirectContact) {
				chara->inDirectContact = true;
				break;
			}
		}
	}
}

void ATurn::SpawnWaitingZombies(FIntPoint tile)
{
	if (zombiesWaitingToEnterGrid.Contains(tile) && zombiesWaitingToEnterGrid[tile].zombies.Num() > 0) {
		AZombie* z = zombiesWaitingToEnterGrid[tile].zombies[0];
		zombiesWaitingToEnterGrid[tile].zombies.Remove(z);
		zombies.Add(z);
		grid->gridTiles[tile].actor = z;
		FVector loc = grid->GetLocationByIndex(tile);
		z->SetActorLocation(loc);
		APlayerCharacter* chara = grid->CharacterInNeighbor(tile);
		if (chara != nullptr) {
			chara->inDirectContact = true;
			z->characterInContact = chara;
		}
	}
}

void ATurn::SpawnZombieAfterMoveCharacter(AZombie* zombie, bool inSearch)
{
	grid->SetZombieStartLocation(zombie);
	zombies.Add(zombie);
	if (inSearch) {
		if (hud != nullptr && hud->PlayerInfoWidget != nullptr) {
			hud->PlayerInfoWidget->UnhidePlayerInfoDuringSearch();
		}
	}
	else {
		EndZombieTurn();
	}
}

void ATurn::SpawnZombieBeforeStartGame()
{
	for (FIntPoint tile : initialSpawnZombiesTiles) {
		if (zombiesToEnterGrid.Num() > 0) {
			AZombie* newZombie = zombiesToEnterGrid[0];
			zombiesToEnterGrid.Remove(newZombie);
			newZombie->startIndex = tile;
			if (grid->gridTiles[tile].actor == nullptr) {
				grid->SetZombieStartLocation(newZombie);
				zombies.Add(newZombie);
			}
		}
	}
}

void ATurn::ResetZombiesWhenAllAreDead()
{
	if (zombiesToEnterGrid.Num() == 0 && zombiesDied.Num() > 0) {
		zombiesToEnterGrid = zombiesDied;
		zombiesDied.Empty();
	}
}

void ATurn::SetBetaMPWhenAlphaOnBoardGetsHit()
{
	for (AZombie* z : zombies) {
		if (z->typeOfZombie == ZombieType::Beta || z->typeOfZombie == ZombieType::BetaCharacter) {
			z->maxmp = 1;
			z->mp = 1;
		}
	}
}

void ATurn::SpawnZombieInCharacterTile(AZombie* newZombie, FIntPoint tile, bool inSearch)
{
	APlayerCharacter* characterInTile = Cast<APlayerCharacter>(grid->gridTiles[tile].actor);
	if (characterInTile != nullptr) {
		if (characterInTile->inDirectContact) {
			FIntPoint t = grid->GetTileIndexFromLocation(characterInTile->GetActorLocation());
			TArray<FIntPoint> neighbors = grid->GetTileNeighbors(t);
			for (FIntPoint index : neighbors) {
				if (grid->gridTiles[index].actor != nullptr) {
					AZombie* zombie = Cast<AZombie>(grid->gridTiles[index].actor);
					if (zombie != nullptr && zombie->characterInContact == characterInTile) {
						zombie->characterInContact = nullptr;
					}
				}
			}
			characterInTile->isLocked = false;
			if (characterInTile->stunIcon != nullptr) {
				characterInTile->stunIcon->SetActorHiddenInGame(true);
			}
		}
		characterInTile->inDirectContact = true;
		newZombie->characterInContact = characterInTile;
		UActionSelectTileToMove* command = NewObject<UActionSelectTileToMove>(this);
		command->Execute(grid, characterInTile);
		APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		UActionSelectTileToMove* commandActions = NewObject<UActionSelectTileToMove>(player->actions);
		commandActions->turn = this;
		commandActions->zombie = newZombie;
		commandActions->inSearch = inSearch;
		player->actions->command = commandActions;
		player->actions->actionTile = tile;
	}
}

void ATurn::SpawnZombieInTile(FIntPoint tile)
{
	if (tile != FIntPoint(-1, -1) && zombiesToEnterGrid.Num() > 0) {
		AZombie* newZombie = zombiesToEnterGrid[0];
		zombiesToEnterGrid.Remove(newZombie);
		newZombie->startIndex = tile;
		if (grid->gridTiles[tile].actor == nullptr) {
			grid->SetZombieStartLocation(newZombie);
			zombies.Add(newZombie);
			DirectContactInSpawnZombie(tile);
		}
		else {
			SpawnZombieInCharacterTile(newZombie, tile, true);
		}
	}
}

void ATurn::SpawnInitialZombieCharacters()
{
	AZombie* zombieCharacter = GetWorld()->SpawnActor<AZombie>(zombieClass, GetActorLocation(), FRotator(0, -90, 0));
	if (zombieCharacter != nullptr) {
		zombieCharacter->typeOfZombie = ZombieType::BetaCharacter;
		zombieCharacter->SetHealthAndMPPropertiesByZombie();
	}
}

