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

UENUM()
enum MovementType
{
	Stationary     UMETA(DisplayName = "Stationary"),
	Walking     UMETA(DisplayName = "Walking"),
	Running   UMETA(DisplayName = "Running"),

};

UENUM()
enum CoveringType
{
	NONE     UMETA(DisplayName = "NONE"),
	CStationary     UMETA(DisplayName = "CStationary"),
	Walked   UMETA(DisplayName = "Walked"),
	Ran     UMETA(DisplayName = "Ran"),

};

USTRUCT(BlueprintType)
struct FTilesLoF
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileLoF)
		TArray<FIntPoint> tilesLoF;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileLoF)
		int distance;

	FTilesLoF()
	{
	};

	FTilesLoF(TArray<FIntPoint> tiles, int d)
	{
		this->tilesLoF = tiles;
		this->distance = d;
	}
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

	UPROPERTY(EditAnywhere) FIntPoint startIndex = FIntPoint(7, 6);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Grid) class AGrid* grid;

	UPROPERTY(VisibleAnywhere, Category = Weapon) TEnumAsByte<EWeapon> PreferredWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) TEnumAsByte<EWeapon> weapon1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) TEnumAsByte<EWeapon> weapon2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) TEnumAsByte<EWeapon> weapon3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) TEnumAsByte<EWeapon> weapon4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) class UWeapon* readyWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon) class UWeapon* readySecondaryWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon) bool useReadyWeapon = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = variables) TArray<FIntPoint> AoF;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = variables) TMap<FIntPoint,FTilesLoF> LoFs;
	UPROPERTY(VisibleAnywhere, Category = Movement) TEnumAsByte<MovementType> typeOfMovement;
	UPROPERTY(VisibleAnywhere, Category = Covering) TEnumAsByte<CoveringType> typeOfCovering;

	//Funciones
	UFUNCTION() void SetPreferredWeaponByCharacter();
	UFUNCTION() void getArcOfFire();
	UFUNCTION() int getDistanceLoF(TArray<FIntPoint> tiles, FIntPoint index);
	UFUNCTION() void AttackZombieHandToHand(class AZombie* zombie, FIntPoint tileZombie);
	UFUNCTION() int GetDistanceAttackHandToHand();
	UFUNCTION() TArray<FIntPoint> GetIndexHandToHand2Range();
	UFUNCTION() int GetNumberOfHitModifiersLoF(FIntPoint tileZombie);
	UFUNCTION() int GetNumberOfHitModifiersAttack(class AZombie* zombie);
	UFUNCTION() int GetPrimaryHitHandToHand();
	UFUNCTION() int GetPrimaryHitAndMultipleFire(FIntPoint tileZombie);
	UFUNCTION() void FriendlyFire(FIntPoint tileZombie);
};
