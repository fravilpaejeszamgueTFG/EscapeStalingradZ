// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM()
enum EWeapon
{
	None     UMETA(DisplayName = "None"),
	MP40     UMETA(DisplayName = "MP-40"),
	Nagant   UMETA(DisplayName = "Nagant"),
	MG34     UMETA(DisplayName = "MG34"),
	Axe      UMETA(DisplayName = "Axe"),
	Knife    UMETA(DisplayName = "Knife"),
	Luger     UMETA(DisplayName = "Luger"),

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ESCAPESTALINGRADZ_API UWeapon : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetPropiertiesNone();
	void SetPropiertiesMP40();
	void SetPropiertiesNagant();
	void SetPropiertiesMG34();
	void SetPropiertiesAxe();
	void SetPropiertiesKnife();
	void SetPropiertiesLuger();

public:	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon) TEnumAsByte<EWeapon> weaponName = EWeapon::None;
	UPROPERTY(VisibleAnywhere) int rateOfFire;
	UPROPERTY(VisibleAnywhere) int primaryHit;
	UPROPERTY(VisibleAnywhere) int pointBlankRange;
	UPROPERTY(VisibleAnywhere) int multipleRange;
	UPROPERTY(VisibleAnywhere) bool oneHandWeapon;
	UPROPERTY(VisibleAnywhere) int hand2handHit;

	//funciones
	UFUNCTION(BlueprintCallable)
		void SetPropiertiesByName(EWeapon weapon);
	UFUNCTION() bool GetIfWeaponIsTwoHandByName(EWeapon weapon);
};
