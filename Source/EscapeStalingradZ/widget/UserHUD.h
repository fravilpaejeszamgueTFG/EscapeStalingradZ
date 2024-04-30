// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EscapeStalingradZ/enum/Enumerates.h"
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

	UPROPERTY(EditDefaultsOnly, Category = "ReleaseLock")
		TSubclassOf<class UWReleaseLock> ReleaseLockWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UWReleaseLock* ReleaseLockWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
		TSubclassOf<class UWInventory> InventoryWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UWInventory* InventoryWidget;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerInfo")
		TSubclassOf<class UWSelectCharacter> SelectCharacterWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerInfo")
		TSubclassOf<class UWSelectCharacterToFree> SelectCharacterToFreeWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "BetweenScenario")
		TSubclassOf<class UWBetweenScenarios> BetweenScenariosWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UWBetweenScenarios* betweenScenariosWidget;

	UPROPERTY(EditDefaultsOnly, Category = "ExchangeEquipment")
		TSubclassOf<class UWExchangeEquipment> ExchangeEquipmentWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UWExchangeEquipment* ExchangeEquipmentWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Death")
		TSubclassOf<class UWDeathScreen> DeathScreenWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UWSettings* SettingsWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
		TSubclassOf<class UWSettings> SettingsWidgetClass;

	UPROPERTY(VisibleAnywhere)
		class ATurn* turn;

	UPROPERTY(VisibleAnywhere, Category = Character) class APlayerCharacter* character;
	UPROPERTY(VisibleAnywhere, Category = Character) class ACharacterToFree* characterToFree;

	UPROPERTY(VisibleAnywhere) TEnumAsByte<EWeapon> favoriteWeaponCharacterToFree;

	UFUNCTION()
		void StartCharacterTurn();
	UFUNCTION()
		void CreateOrSetPlayerInfo();
	UFUNCTION()
		void CreateOrSetReleaseLock();
	UFUNCTION()
		void CreateOrSetInventory();
	UFUNCTION()
		void HidePlayerInfo();
	UFUNCTION()
		void StartGameAfterSelectCharacter();
	UFUNCTION()
		void CreateSelectCharacterWidget(class APlayerCharacter* chara);
	UFUNCTION()
		void CreateSelectCharacterToFreeWidget(class APlayerCharacter* chara);
	UFUNCTION()
		void SetPrimaryCharacterToBetweenScenarios(class APlayerCharacter* chara);
	UFUNCTION()
		void SetSecondaryCharacterToBetweenScenarios(class APlayerCharacter* chara);
	UFUNCTION()
		void CreateOrSetExchangeEquipment(class APlayerCharacter* character1, class APlayerCharacter* character2);
	UFUNCTION()
		void EndGame();
	UFUNCTION()
		void CreateOrSetSelectSettingsWidget();
};
