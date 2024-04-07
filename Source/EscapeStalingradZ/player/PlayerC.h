// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerC.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API APlayerC : public APlayerController
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	APlayerC();

	UPROPERTY(VisibleAnywhere, Category = movement)
		class UWSelectMovementType* Movement;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UWSelectMovementType> MovementClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool hasPrimaryObjective = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool hasSecondaryObjective = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool canExitTheRoom = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = actions) class UPlayerActions* actions;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character) class APlayerCharacter* playerchara;

	UPROPERTY(VisibleAnywhere, Category = Exits) TArray<class AExitModifier*> exits;

	UFUNCTION() void LeftMouseClick();

	UFUNCTION() void SetMovementWidget();

	UFUNCTION() void HideMovementWidget();
	
};
