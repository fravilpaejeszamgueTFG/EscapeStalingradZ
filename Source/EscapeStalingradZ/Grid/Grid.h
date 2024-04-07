// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EscapeStalingradZ/enum/Enumerates.h"
#include "Grid.generated.h"

USTRUCT(BlueprintType)
struct FTileData
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = TileData)
		FIntPoint index;
	UPROPERTY(VisibleAnywhere, Category = TileData)
		TArray<TEnumAsByte<TileType>>  types;
	UPROPERTY(VisibleAnywhere, Category = TileData)
		bool  poisoned;
	UPROPERTY(VisibleAnywhere, Category = TileData)
		class AActor* actor = nullptr;
	UPROPERTY(VisibleAnywhere, Category = TileData)
		TArray<TEnumAsByte<TileState>> states;
	UPROPERTY(VisibleAnywhere, Category = TileData)
		TArray<FIntPoint> walls;
	UPROPERTY(VisibleAnywhere, Category = TileData)
		TMap<FIntPoint, bool> doors; //si bool = true -> puerta cerrada, si bool = false -> puerta abierta

	FTileData() { 
		poisoned = false;
	};

	FTileData(FIntPoint index, TileType type, bool poisoned)
	{
		this->index = index;
		this->types.Add(type);
		this->poisoned = poisoned;
	}
};

UCLASS()
class ESCAPESTALINGRADZ_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) USceneComponent* DefaultSceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) class UInstancedStaticMeshComponent* instancedMesh;
	//particula para LoF
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true")) class UParticleSystemComponent* particleLoF;

	//variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = variables) FVector centerOfGrid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = variables) FVector tileScale = FVector(100, 100, 0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = variables) FVector2D numberOfTiles = FVector2D(8,8);
	UPROPERTY(VisibleAnywhere, Category = variables) FVector gridBottomLeftCornerLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = variables) FVector meshSize = FVector(100, 100, 1);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = variables) TMap<FIntPoint, FTileData> gridTiles;
	//colores de casillas
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = colors) FLinearColor tileNoneColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = colors) FLinearColor tileHoveredColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = colors) FLinearColor tileSelectedColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = colors) FLinearColor tileNeighborColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = colors) FLinearColor tileReachableColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = colors) FLinearColor tileAoFColor;

	//nombre del mapa
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = scenario) TEnumAsByte<ScenarioName> name;

	//actores
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = characters) TArray<FIntPoint> charactersIndex;

	//functions
	UFUNCTION(BlueprintCallable) 
		void SpawnGrid(FVector center, FVector tileSize, FVector2D numOfTiles);
	UFUNCTION() 
		FVector GetCursorLocationOnGrid();
	UFUNCTION() 
		FIntPoint GetTileIndexFromLocation(FVector location);
	UFUNCTION(BlueprintCallable)
		FIntPoint GetTileIndexUnderCursor();
	UFUNCTION(BlueprintCallable)
		void ChangeTileData(FIntPoint index, FTileData data);
	UFUNCTION(BlueprintCallable)
		void AddTileState(FIntPoint index, TileState state);
	UFUNCTION(BlueprintCallable)
		void RemoveTileState(FIntPoint index, TileState state);
	UFUNCTION(BlueprintCallable)
		void UpdateTileVisual(FIntPoint index);
	UFUNCTION(BlueprintCallable)
		FLinearColor GetColorFromState(TArray<TEnumAsByte<TileState>> states);
	UFUNCTION(BlueprintCallable)
		TArray<FIntPoint> GetTileNeighbors(FIntPoint index);
	UFUNCTION()
		TArray<FIntPoint> GetFrontTiles(FIntPoint index, FVector forwardVector, FVector rightVector);
	UFUNCTION()
		void UpdateTileNeighbors(FIntPoint index, bool isadding);
	UFUNCTION(BlueprintCallable)
		void SetPlayerStartLocation(class APlayerCharacter* character);
	UFUNCTION(BlueprintCallable)
		void SetZombieStartLocation(class AZombie* zombie);
	UFUNCTION()
		FIntPoint GetStartIndex();
	UFUNCTION(BlueprintCallable)
		FVector GetLocationByIndex(FIntPoint index);

	UFUNCTION()
		TArray<FIntPoint> GetTilesForward(FIntPoint index, FVector forwardVector, int num_casillas);
	UFUNCTION()
		TArray<FIntPoint> GetTilesForwardMovement(FIntPoint index, FVector forwardVector, int num_casillas, int mp, int cost);
	UFUNCTION()
		void SetTilesForAttack(TArray<FIntPoint> list);
	UFUNCTION()
		TArray<FIntPoint> GetTilesAoF(FIntPoint index, FVector forwardVector, FVector rightVector);
	UFUNCTION()
		int GetDistanceAoF(FIntPoint index, FVector forwardVector);
	UFUNCTION()
		TArray<FIntPoint> GetAdjacentForward(FIntPoint index, int iterator, FVector rightVector);
	UFUNCTION(BlueprintCallable)
		TArray<FIntPoint> GetTilesLoF(FIntPoint start, FIntPoint end);
	UFUNCTION()
		TArray<FIntPoint> GetTilesDiagonals(FIntPoint index, FVector forwardVector, FVector rightVector, int numCasillas, int mp);
	UFUNCTION()
		TArray<FIntPoint> GetTilesRotation(FIntPoint index, FVector forwardVector, FVector rightVector, int mp);
	UFUNCTION(BlueprintCallable)
		void SetParticleLoF(FIntPoint start, FIntPoint end);
	UFUNCTION(BlueprintCallable)
		void DeActivateParticleLoF();
	UFUNCTION()
		TArray<FIntPoint> GetTilesWithZombies(TArray<FIntPoint> AoF);
	UFUNCTION()
		bool GetDoorIsClosed(FIntPoint index, FIntPoint other);
	UFUNCTION()
		bool CanMoveDiagonal(FIntPoint tile, FIntPoint forward, FIntPoint right, FIntPoint backward);
	UFUNCTION()
		bool CanShootDiagonal(FIntPoint tile, FIntPoint forward, FIntPoint right, FIntPoint backward);
	UFUNCTION()
		bool DoorOrWallBetweenTiles(FIntPoint tile1, FIntPoint tile2);
	UFUNCTION()
		void deleteStatesFromTiles();
	UFUNCTION()
		void deleteStatesFromTilesButSelected();

	UFUNCTION()
		void DeleteStatesFromTilesButGiven(TArray<FIntPoint> list);

	UFUNCTION()
		int GetCostToEnterNeighbor(FIntPoint index, FIntPoint neighbor);

	UFUNCTION()
		bool HasZombieInNeighbor(FIntPoint index);

	UFUNCTION()
		class AZombie* ZombieInNeighbor(FIntPoint index);

	UFUNCTION()
		class APlayerCharacter* CharacterInNeighbor(FIntPoint index);

	UFUNCTION()
		TArray<FIntPoint> GetTileNeighborsThatCanMoveInto(FIntPoint index);

	UFUNCTION()
		void EndIfTileIsExit(FIntPoint index);
};
