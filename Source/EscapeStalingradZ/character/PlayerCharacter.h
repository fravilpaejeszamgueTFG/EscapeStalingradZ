// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EscapeStalingradZ/weapon/Weapon.h"
#include "PlayerCharacter.generated.h"

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Grid) class AGrid* grid;

	UPROPERTY(VisibleAnywhere, Category = Weapon) TEnumAsByte<EWeapon> PreferredWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) TEnumAsByte<EWeapon> weapon1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) TEnumAsByte<EWeapon> weapon2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) TEnumAsByte<EWeapon> weapon3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) TEnumAsByte<EWeapon> weapon4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) class UWeapon* readyWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) class UWeapon* readySecondaryWeapon;

	//Funciones
	void SetPreferredWeaponByCharacter();
	UFUNCTION() void getArcOfFire();
};
