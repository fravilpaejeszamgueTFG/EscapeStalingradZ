// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EscapeStalingradZ/weapon/Weapon.h"
#include "EscapeStalingradZ/enum/Enumerates.h"
#include "PlayerCharacter.generated.h"

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack) bool attacked = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player) bool isPrimaryPlayer = true;

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
	UPROPERTY(VisibleAnywhere, Category = Covering) TEnumAsByte<CoveringAttackType> typeOfCoveringAttack;
	UPROPERTY(VisibleAnywhere, Category = DirectContact) bool inDirectContact = false;
	UPROPERTY(VisibleAnywhere, Category = DirectContact) bool isLocked = false;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		TSubclassOf<class UWDicesCombat> DicesCombatWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		class UWDicesCombat* DicesCombatWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		TSubclassOf<class UWDiceSpreadCombat> DiceSpreadCombatWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		class UWDiceSpreadCombat* DiceSpreadCombatWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		TSubclassOf<class UWSearchDie> SearchDieWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		class UWSearchDie* SearchDieWidget;

	//Funciones
	UFUNCTION() void SetPreferredWeaponByCharacter();
	UFUNCTION() void getArcOfFire();
	UFUNCTION() int getDistanceLoF(TArray<FIntPoint> tiles, FIntPoint index);
	//Ataque normal
	UFUNCTION() void AttackZombieNormalFire(class AZombie* zombie, FIntPoint tileZombie);
	UFUNCTION() void AttackZombieSpreadFire(class AZombie* zombie, FIntPoint tileZombie);
	UFUNCTION() void MoveToTileWithZombieDuringSpreadFire(class AZombie* zombie, FIntPoint tileZombie);
	UFUNCTION() void MoveToTileWithoutZombieDuringSpreadFire(FIntPoint destinyTile);
	UFUNCTION() void AttackZombieHandToHand(class AZombie* zombie, FIntPoint tileZombie);
	//coveringAttack
	UFUNCTION() void CoveringAttackHandToHand(class AZombie* zombie, FIntPoint tileZombie);
	UFUNCTION() void CoveringAttackNormalFire(class AZombie* zombie, FIntPoint tileZombie);
	UFUNCTION() void CoveringAttackSpreadFire(class AZombie* zombie, FIntPoint tileZombie);
	//auxiliares ataque
	UFUNCTION() TArray<int> GetDice(int numberOfDice);
	UFUNCTION() int GetDistanceAttackHandToHand();
	UFUNCTION() TArray<FIntPoint> GetIndexHandToHand2Range();
	UFUNCTION() int GetNumberOfHitModifiersLoF(FIntPoint tileZombie);
	UFUNCTION() FIntPoint GetForwardIndexInDiagonal(FIntPoint start, FIntPoint end);
	UFUNCTION() int GetNumberOfHitModifiersAttack(class AZombie* zombie);
	UFUNCTION() int GetPrimaryHitHandToHand();
	UFUNCTION() int GetPrimaryHitAndMultipleFire(FIntPoint tileZombie);
	UFUNCTION() void FriendlyFire(FIntPoint tileZombie);
	//dados ataques
	UFUNCTION() int GetNumberOfDices();
	UFUNCTION() void CreateOrSetDicesCombatWidget(class AZombie* zombie, TArray<int> dice, int targetDie, bool isHandToHand, bool inCovering);
	UFUNCTION() void CreateOrSetSpreadCombateWidget(class AZombie* zombie, TArray<int> dice, int targetDie, bool inCovering);
	UFUNCTION() void FriendlyFireGivenZombie(class AZombie* zombie);
	//fijado zombie
	UFUNCTION() void ZombieLock(class AZombie* zombie);
	UFUNCTION() FRotator GetRotationDirectionToZombie(class AZombie* zombie);
	//busqueda
	UFUNCTION() void SearchAction();
	UFUNCTION() void CreateOrSetDieSearchWidget(int numberOfDie);
	//setear imagen
	UFUNCTION(BlueprintImplementableEvent) void SetImage();
	//inicio turno al cambiar de nivel
	FTimerHandle initialTimer;
	UFUNCTION() void SetInitialTurn();
};
