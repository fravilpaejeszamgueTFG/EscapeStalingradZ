// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerActions.generated.h"

UCLASS()
class ESCAPESTALINGRADZ_API APlayerActions : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerActions();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//variables
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Grid) class AGrid* grid;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Tile) FIntPoint hoveredTile;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Tile) FIntPoint selectedTile = FIntPoint(-1,-1);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Tile) FIntPoint actionTile = FIntPoint(-1, -1);
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Command) class UCommand* command;

	UFUNCTION()
		void LeftMouseClick();
	UFUNCTION()
		void RightMouseClick();
	UFUNCTION()
		void UpdateHoveredTile();
};
