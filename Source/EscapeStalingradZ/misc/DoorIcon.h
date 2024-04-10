// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorIcon.generated.h"

UCLASS()
class ESCAPESTALINGRADZ_API ADoorIcon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorIcon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FIntPoint index1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FIntPoint index2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) bool isOpen = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) bool isRotated = false;

	UFUNCTION() void ChangeValueIsOpen();
	UFUNCTION(BlueprintImplementableEvent) void SetImage();

};
