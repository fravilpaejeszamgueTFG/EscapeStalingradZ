// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turn.generated.h"

USTRUCT(BlueprintType)
struct FZombiesWaiting
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ZombiesWaiting)
		TArray<class AZombie*> zombies;

	FZombiesWaiting()
	{
	};

	FZombiesWaiting(TArray<class AZombie*> zombiesw)
	{
		this->zombies = zombiesw;
	}
};

UCLASS()
class ESCAPESTALINGRADZ_API ATurn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) USceneComponent* DefaultSceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) class UBoxComponent* boxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Grid) class AGrid* grid;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = characters) TArray<class APlayerCharacter*> characters;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = characters) TArray<class APlayerCharacter*> charactersToStartTurn;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = characters) class APlayerCharacter* selectedCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = turn) bool endTurnHuman = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = zombies) TArray<class AZombie*> zombies;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = zombies) TArray<class AZombie*> zombiesToStartTurn;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = zombies) TArray<class AZombie*> zombiesDied;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = zombies) TArray<class AZombie*> zombiesToEnterGrid;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = zombies) TMap<FIntPoint, FZombiesWaiting> zombiesWaitingToEnterGrid;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = zombies) class AZombie* selectedZombie;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = turn) bool endTurnZombie = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = turn) int roundNumber = 0;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Map) bool isFubar = true;
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = turn)
		TArray<FIntPoint> spawnZombiesTiles; //indica donde poner un zombie en la ronda del indice del array. Si FIntPoint = -1,-1 -> no spawnea.
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = turn)
		TArray<FIntPoint> initialSpawnZombiesTiles; //casillas donde deben spawnear zombies al empezar el tablero.

	UPROPERTY(VisibleAnywhere)
		class UWSelectCharacterTurn* turnWidget;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UWSelectCharacterTurn> turnWidgetClass;

	UPROPERTY(VisibleAnywhere)
		class UWDicesTurn* turnDicesWidget;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UWDicesTurn> turnDicesWidgetClass;

	UPROPERTY(VisibleAnywhere)
		class AUserHUD* hud;

	UFUNCTION(BlueprintCallable)
		void SetCharacters(TArray<FIntPoint>  charactersList);

	UFUNCTION()
		void nextCharacter();

	UFUNCTION()
		void SetNextCharacter();

	UFUNCTION()
		void CreateSelectCharacterTurnWidget();

	FTimerHandle WaitTimer;

	UFUNCTION()
		void ActivateCollision();

	UFUNCTION()
		void nextZombie();

	UFUNCTION()
		void SetNextZombie();

	UFUNCTION()
		void SpawnZombies();

	UFUNCTION()
		void EndZombieTurn();

	UFUNCTION()
		void StartTurn();

	UFUNCTION()
		void ZombiesStartTurn();

	UFUNCTION()
		void Initiative();

	UFUNCTION()
		void EndTurn();

	UFUNCTION()
		void CreateOrSetTurnDicesWidget(int humanDie, int targetDie);

	UFUNCTION() void RemoveStunToZombies();
	UFUNCTION() void PrepareCharacterForTurn();
	UFUNCTION() void DirectContactInSpawnZombie(FIntPoint tile);
	UFUNCTION() void SpawnWaitingZombies(FIntPoint tile);
	UFUNCTION() void SpawnZombieAfterMoveCharacter(class AZombie* zombie);
	UFUNCTION() void SpawnZombieBeforeStartGame();
	UFUNCTION() void ResetZombiesWhenAllAreDead();

};
