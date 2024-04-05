// Fill out your copyright notice in the Description page of Project Settings.


#include "Turn.h"
#include "Components/BoxComponent.h"
#include "EscapeStalingradZ/grid/Grid.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "EscapeStalingradZ/widget/WSelectCharacterTurn.h"
#include "EscapeStalingradZ/player/PlayerActions.h"
#include "Kismet/GameplayStatics.h"
#include "EscapeStalingradZ/player/PlayerC.h"
#include "EscapeStalingradZ/widget/WSelectMovementType.h"
#include "EscapeStalingradZ/widget/UserHUD.h"
#include "EscapeStalingradZ/widget/WDicesTurn.h"
#include "EscapeStalingradZ/zombies/Zombie.h"
#include "EscapeStalingradZ/player/actions/ActionSelectTileToMove.h"

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
			UE_LOG(LogTemp, Warning, TEXT("Fin de partida"));
		}
		else {
			hud->HidePlayerInfo();
			charactersToStartTurn = characters;
			endTurnHuman = true;
			UE_LOG(LogTemp, Warning, TEXT("Turno humano terminado"));
			if (endTurnZombie) {
				EndTurn();
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Empieza turno zombie"));
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
		UE_LOG(LogTemp, Warning, TEXT("Turno zombie empieza"));
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
					APlayerCharacter* characterInTile = Cast<APlayerCharacter>(grid->gridTiles[tile].actor);
					if (characterInTile != nullptr) {
						characterInTile->health--; //pasar a una funcion en character que compruebe si muere
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
						}
						characterInTile->inDirectContact = true;
						newZombie->characterInContact = characterInTile;
						UActionSelectTileToMove* command = NewObject<UActionSelectTileToMove>(this);
						command->Execute(grid, characterInTile);
						APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
						UActionSelectTileToMove* commandActions = NewObject<UActionSelectTileToMove>(player->actions);
						commandActions->turn = this;
						commandActions->zombie = newZombie;
						player->actions->command = commandActions;
						player->actions->actionTile = tile;
					}
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
		UE_LOG(LogTemp, Warning, TEXT("Empieza turno humano"));
	}
}

void ATurn::StartTurn()
{
	if (isFubar) {
		if (false) { //TO-DO si activa tutorial
			if (roundNumber == 0) {
				//TO-DO Hacer tutorial primera ronda (movimientos basicos)
			}
			else if (roundNumber == 1) {
				//TO-DO Hacer tutorial segunda ronda (buscar objeto)
			}
			else if (roundNumber == 2) {
				//TO-DO Hacer tutorial tercera ronda (acciones (rotar,abrir/cerrar puerta))
			}
			else if (roundNumber == 3) {
				//TO-DO Hacer tutorial en cuarta ronda (iniciativa y combate)
			}
			else if (roundNumber > 3) {
				Initiative();
			}
		}
		else {
			Initiative();
		}
	}
	else {
		Initiative();
	}
}

void ATurn::ZombiesStartTurn()
{
	zombiesToStartTurn = zombies;
	SetNextZombie();
}

void ATurn::Initiative()
{
	charactersToStartTurn = characters;
	//empieza jugando el jugador
	int humanDie = FMath::RandRange(1, 12);
	int zombieDie = FMath::RandRange(1, 12);
	int finalZombieDie = zombieDie - 4;
	for (AZombie* z : zombies) {
		if (z->typeOfZombie == ZombieType::Alpha) {
			finalZombieDie += 4;
			break;
		}
	}
	CreateOrSetTurnDicesWidget(humanDie, finalZombieDie);
}

void ATurn::EndTurn()
{
	endTurnZombie = false;
	endTurnHuman = false;
	roundNumber++;
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

void ATurn::SpawnZombieAfterMoveCharacter(AZombie* zombie)
{
	int numRound = roundNumber % spawnZombiesTiles.Num();
	FIntPoint tile = spawnZombiesTiles[numRound];
	grid->SetZombieStartLocation(zombie);
	zombies.Add(zombie);
	EndZombieTurn();
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
	if (zombiesToEnterGrid.Num() == 0 && zombies.Num() == 0 && zombiesDied.Num() > 0) {
		zombiesToEnterGrid = zombiesDied;
		zombiesDied.Empty();
	}
}

