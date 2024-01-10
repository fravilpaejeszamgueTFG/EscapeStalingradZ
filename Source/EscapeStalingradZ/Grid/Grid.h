// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"


UENUM()
enum TileType
{
	Normal     UMETA(DisplayName = "Normal"),
	Hinder     UMETA(DisplayName = "Hinder"),
	Fire	   UMETA(DisplayName = "Fire"),
	Search	   UMETA(DisplayName = "Search"),
	Spawn	   UMETA(DisplayName = "Spawn"),
	Exit	   UMETA(DisplayName = "Exit")
};

UENUM()
enum TileState
{
	Hovered     UMETA(DisplayName = "Hovered"),
	Selected	   UMETA(DisplayName = "Selected"),
	isNeighbor		UMETA(DisplayName = "isNeighbor"),
	isReachable		UMETA(DisplayName = "isReachable"),
	isInAoF			UMETA(DisplayName = "isInAoF"),
};

USTRUCT(BlueprintType)
struct FTileData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileData)
		FIntPoint index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileData)
		TEnumAsByte<TileType>  type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileData)
		bool  poisoned;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileData)
		class AActor* actor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileData)
		TArray<TEnumAsByte<TileState>> states;

	FTileData() { 
		type = TileType::Normal;
		poisoned = false;
	};

	FTileData(FIntPoint index, TileType type, bool poisoned)
	{
		this->index = index;
		this->type = type;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = variables) TMap<FIntPoint, FTileData> gridTiles;
	//colores de casillas
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = colors) FLinearColor tileNoneColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = colors) FLinearColor tileHoveredColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = colors) FLinearColor tileSelectedColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = colors) FLinearColor tileNeighborColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = colors) FLinearColor tileReachableColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = colors) FLinearColor tileAoFColor;

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
		void UpdateTileNeighbors(FIntPoint index, bool isadding);
	UFUNCTION(BlueprintCallable)
		void SetPlayerStartLocation(class APlayerCharacter* character);
	UFUNCTION(BlueprintCallable)
		void SetZombieStartLocation(class AZombie* zombie);
	UFUNCTION()
		FIntPoint GetStartIndex();
	UFUNCTION(BlueprintCallable)
		FVector GetLocationByIndex(FIntPoint index);

	UFUNCTION(BlueprintCallable)
		TArray<FIntPoint> GetTilesForward(FIntPoint index, FVector forwardVector, int num_casillas);

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
	UFUNCTION(BlueprintCallable)
		void SetParticleLoF(FVector start, FVector end);
	UFUNCTION(BlueprintCallable)
		void DeActivateParticleLoF();
	UFUNCTION()
		TArray<FIntPoint> GetTilesWithZombies(TArray<FIntPoint> AoF);
};
