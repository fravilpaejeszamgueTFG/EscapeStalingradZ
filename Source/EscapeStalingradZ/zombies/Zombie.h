// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EscapeStalingradZ/enum/Enumerates.h"
#include "Zombie.generated.h"

USTRUCT(BlueprintType)
struct FPathFinding
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = TileData)
		FIntPoint index;
	UPROPERTY(VisibleAnywhere, Category = TileData)
		int costToEnterTile;
	UPROPERTY(VisibleAnywhere, Category = TileData)
		int costFromStart;
	UPROPERTY(VisibleAnywhere, Category = TileData)
		int minimumCostToTarget;
	UPROPERTY(VisibleAnywhere, Category = TileData)
		FIntPoint previousIndex;
	

	FPathFinding() {
		index = FIntPoint(-1, -1);
		costToEnterTile = 1;
		costFromStart = 99999;
		minimumCostToTarget = 99999;
		previousIndex = FIntPoint(-1, -1);
	};

	FPathFinding(FIntPoint i, int costToEnter, int InitialCost, int minimumCost, FIntPoint previousI) {
		index = i;
		costToEnterTile = costToEnter;
		costFromStart = InitialCost;
		minimumCostToTarget = minimumCost;
		previousIndex = previousI;
	};
};

UCLASS()
class ESCAPESTALINGRADZ_API AZombie : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) USceneComponent* DefaultSceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) class UStaticMeshComponent* mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) class UWidgetComponent* zombieInfo;

	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ZombieType) TEnumAsByte<ZombieType> typeOfZombie;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) int health = 1; //Setear
	UPROPERTY(VisibleAnywhere) int dmg = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int mp = 1; //Setear
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int maxmp = 1; //Setear
	UPROPERTY(EditAnywhere) FIntPoint startIndex = FIntPoint(5,1);
	UPROPERTY(VisibleAnywhere) bool isStunned = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Grid) class AGrid* grid;

	UPROPERTY(VisibleAnywhere, Category = Turn) class ATurn* turn;

	UPROPERTY(VisibleAnywhere, Category = characters) TArray<class APlayerCharacter*> characters;

	UPROPERTY(VisibleAnywhere, Category = characters) TArray<class APlayerCharacter*> charactersInCovering;

	UPROPERTY(VisibleAnywhere, Category = PathFinding) TArray<FIntPoint> pathToCharacter;

	UPROPERTY(VisibleAnywhere, Category = PathFinding) TArray<FIntPoint> discoveredTiles;

	UPROPERTY(VisibleAnywhere, Category = PathFinding) TArray<int> discoveredTilesSortingCosts;

	UPROPERTY(VisibleAnywhere, Category = PathFinding) TArray<FIntPoint> analysedTiles;

	UPROPERTY(VisibleAnywhere, Category = PathFinding) FPathFinding currentDiscoveredTile;

	UPROPERTY(VisibleAnywhere, Category = PathFinding) TArray<FPathFinding> currentNeighbors;

	UPROPERTY(VisibleAnywhere, Category = PathFinding) FPathFinding currentNeighbor;

	UPROPERTY(VisibleAnywhere, Category = PathFinding) TMap<FIntPoint, FPathFinding> pathFindingData;

	UPROPERTY(VisibleAnywhere, Category = PathFinding) FIntPoint target;

	UPROPERTY(VisibleAnywhere, Category = Movement) FVector currentLocation;

	UPROPERTY(VisibleAnywhere, Category = Movement) FVector targetLocation;

	UPROPERTY(EditAnywhere, Category = Movement) int currentIndexPath = 0;

	FTimerHandle movementTimer;

	UPROPERTY(VisibleAnywhere, Category = Movement) float currentMovementTime = 0.;

	UPROPERTY(VisibleAnywhere, Category = Movement) int stepMovementPerSecond = 60;

	UPROPERTY(VisibleAnywhere, Category = Movement) int cont = 0;

	UPROPERTY(VisibleAnywhere, Category = Movement) float timeMovement = 2.f;

	UPROPERTY(VisibleAnywhere, Category = DirectContact) class APlayerCharacter* characterInContact;

	UPROPERTY(EditDefaultsOnly, Category = "Parameters")
		TSubclassOf<class AAnimatedTextAttack> textClass;
	
	//Funciones
	void SetHealthAndMPPropertiesByZombie();

	UFUNCTION() bool ZombieHit(int die, int stunNumber); //=true si muere el zombie, false si no muere
	UFUNCTION() void ZombieActions();
	UFUNCTION() void MovementZombie();
	UFUNCTION() bool MoveZombie(FIntPoint targetTile);
	UFUNCTION() void MoveZombieLocation();
	UFUNCTION() void MoveZombieToNextLocation(int indexInPath);
	UFUNCTION() void CoveringAttack();
	UFUNCTION() bool ZombieDirectContact(); //=true si hay un personaje en una casilla adyacente
	UFUNCTION() void CoveringAttackBeforeLock();
	UFUNCTION() void CheckIfAreCharactersInNeighborWhenStunOrKillZombie(FVector location);
	UFUNCTION() void FreeCharacterWhenStunOrKillZombie(class APlayerCharacter* chara);

	//PathFinding
	UFUNCTION() TArray<FIntPoint> FindPath(FIntPoint start, FIntPoint end);
	UFUNCTION() void DiscoverTile(FPathFinding pathFinding);
	UFUNCTION() int GetMinimunCostBetweenTwoTiles(FIntPoint start, FIntPoint end);
	UFUNCTION() bool AnalyseNextDiscoverTile();
	UFUNCTION() TArray<FIntPoint> GeneratePath(FIntPoint start, FIntPoint end);
	UFUNCTION() FPathFinding PullOutCheapestTileOfDiscoveredList();
	UFUNCTION() TArray<FPathFinding> GetValidNeighbors(FIntPoint index);
	UFUNCTION() bool NeighborIsTarget();
	UFUNCTION() void InsertTileInDiscoveredTiles(FPathFinding pathFinding);
	UFUNCTION() void ClearPathFindingData();
};
