// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridModifier.generated.h"

UCLASS()
class ESCAPESTALINGRADZ_API AGridModifier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridModifier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TileData)
		TEnumAsByte<enum TileType>  type;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = colors) FLinearColor modifierColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) USceneComponent* DefaultSceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) class UStaticMeshComponent* mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) class UBoxComponent* boxCollision;

	FTimerHandle modifierTimer;

	UFUNCTION() 
		void ActivateCollision();
	UFUNCTION()
		void OnOverlap(AActor* me, AActor* other);

};
