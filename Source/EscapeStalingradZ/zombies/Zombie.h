// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Zombie.generated.h"

UENUM()
enum ZombieType
{
	Beta     UMETA(DisplayName = "Beta"),
	Armoured     UMETA(DisplayName = "Armoured"),
	Kugelfisch   UMETA(DisplayName = "Kugelfisch"),
	Alpha     UMETA(DisplayName = "Alpha"),

};

UCLASS()
class ESCAPESTALINGRADZ_API AZombie : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) USceneComponent* DefaultSceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) class UStaticMeshComponent* mesh;

	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ZombieType) TEnumAsByte<ZombieType> typeOfZombie;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) int health = 1; //Setear
	UPROPERTY(VisibleAnywhere) int dmg = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int mp = 1; //Setear
	UPROPERTY(EditAnywhere) FIntPoint startIndex = FIntPoint(5,1);
	UPROPERTY(VisibleAnywhere) bool isStunned = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Grid) class AGrid* grid;

	//Funciones
	void SetHealthAndMPPropertiesByZombie();
};
