// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerActions.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ESCAPESTALINGRADZ_API UPlayerActions : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UPlayerActions();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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
