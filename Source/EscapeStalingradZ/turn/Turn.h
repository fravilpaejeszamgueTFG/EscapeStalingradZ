// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turn.generated.h"

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Grid) class AGrid* grid;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = characters) TArray<class APlayerCharacter*> characters;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = characters) TArray<class APlayerCharacter*> charactersToStartTurn;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = characters) class APlayerCharacter* selectedCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = turn) bool endTurnHuman = false;

	UPROPERTY(VisibleAnywhere)
		class UWSelectCharacterTurn* turnWidget;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UWSelectCharacterTurn> turnWidgetClass;

	UFUNCTION(BlueprintCallable)
		void SetCharacters(TArray<FIntPoint>  charactersList);

	UFUNCTION()
		void nextCharacter();

	UFUNCTION()
		void SetNextCharacter();

	UFUNCTION()
		void CreateSelectCharacterTurnWidget();

};
