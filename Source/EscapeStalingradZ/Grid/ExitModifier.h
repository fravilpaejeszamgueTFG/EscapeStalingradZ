// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EscapeStalingradZ/enum/Enumerates.h"
#include "ExitModifier.generated.h"

UCLASS()
class ESCAPESTALINGRADZ_API AExitModifier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExitModifier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TileData)
		TEnumAsByte<TileType>  type;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) USceneComponent* DefaultSceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) class UBoxComponent* boxCollision;

	FTimerHandle modifierTimer;

	UFUNCTION()
		void ActivateCollision();
	UFUNCTION()
		void OnOverlap(AActor* me, AActor* other);

};
