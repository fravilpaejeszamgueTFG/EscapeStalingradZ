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
		bool  occupied;

	FTileData() { 
		type = TileType::Normal;
		poisoned = false;
		occupied = false;
	};

	FTileData(FIntPoint index, TileType type, bool poisoned, bool occupied)
	{
		this->index = index;
		this->type = type;
		this->poisoned = poisoned;
		this->occupied = occupied;
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

	//variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = variables) FVector centerOfGrid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = variables) FVector tileScale = FVector(100, 100, 0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = variables) FVector2D numberOfTiles = FVector2D(8,8);
	UPROPERTY(VisibleAnywhere, Category = variables) FVector gridBottomLeftCornerLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = variables) FVector meshSize = FVector(100, 100, 1);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = variables) TMap<FIntPoint, FTileData> gridTiles;

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

};
