// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UserHUD.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API AUserHUD : public AHUD
{
	GENERATED_BODY()

public:
	AUserHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerInfo")
		TSubclassOf<class UWPlayerInfo> PlayerInfoWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UWPlayerInfo* PlayerInfoWidget;

	UPROPERTY(VisibleAnywhere)
		class ATurn* turn;

	UPROPERTY(VisibleAnywhere, Category = Character) class APlayerCharacter* character;
	UPROPERTY(VisibleAnywhere, Category = Character) class APlayerC* controller;

	UFUNCTION()
		void CreateOrSetPlayerInfo();
	UFUNCTION()
		void HidePlayerInfo();

};
