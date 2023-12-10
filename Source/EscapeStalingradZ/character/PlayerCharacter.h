// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCharacter.generated.h"

UENUM()
enum Weapon
{
	None     UMETA(DisplayName = "None"),
	MP40     UMETA(DisplayName = "MP-40"),
	Nagant   UMETA(DisplayName = "Nagant"),
	MG34     UMETA(DisplayName = "MG34"),
	Axe      UMETA(DisplayName = "Axe"),
	Knife    UMETA(DisplayName = "Knife"),
	Luger     UMETA(DisplayName = "Luger"),

};

UENUM()
enum AvailableCharacter
{
	Becker     UMETA(DisplayName = "Becker"),
	Dasha     UMETA(DisplayName = "Dasha"),
	Katyusha   UMETA(DisplayName = "Katyusha"),
	Til     UMETA(DisplayName = "Til"),

};

UCLASS()
class ESCAPESTALINGRADZ_API APlayerCharacter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) USceneComponent* DefaultSceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) class UStaticMeshComponent* mesh;

	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvailableCharacter) TEnumAsByte<AvailableCharacter> characterChosen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) int health = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int fatigue = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int exp = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int ammo = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int food = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int medkit = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int mp = 4;

	UPROPERTY(VisibleAnywhere, Category = Weapon) TEnumAsByte<Weapon> PreferredWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) TEnumAsByte<Weapon> weapon1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) TEnumAsByte<Weapon> weapon2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) TEnumAsByte<Weapon> weapon3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) TEnumAsByte<Weapon> weapon4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) TEnumAsByte<Weapon> readyWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) TEnumAsByte<Weapon> readySecondaryWeapon;

	//Funciones
	void SetPreferredWeaponByCharacter();
};
