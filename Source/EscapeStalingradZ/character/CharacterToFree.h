// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EscapeStalingradZ/enum/Enumerates.h"
#include "CharacterToFree.generated.h"

UCLASS()
class ESCAPESTALINGRADZ_API ACharacterToFree : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterToFree();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) USceneComponent* DefaultSceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) class UStaticMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AvailableCharacter) TEnumAsByte<AvailableCharacter> characterChosen;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FIntPoint startIndex = FIntPoint(0, 3);

	UFUNCTION(BlueprintImplementableEvent) void SetImage();

};
