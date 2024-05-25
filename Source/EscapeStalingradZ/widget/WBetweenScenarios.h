// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EscapeStalingradZ/enum/Enumerates.h"
#include "WBetweenScenarios.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWBetweenScenarios : public UUserWidget
{
	GENERATED_BODY()

public:

	UWBetweenScenarios(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* health;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AmmoNumber;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* FoodNumber;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MedKitNumber;
	UPROPERTY(meta = (BindWidget))
		class UButton* useMedKit;
	UPROPERTY(meta = (BindWidget))
		class UButton* useFood;
	UPROPERTY(meta = (BindWidget))
		class UButton* inventory;
	UPROPERTY(meta = (BindWidget))
		class UButton* changeEquipment;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonLeft;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonRight;
	UPROPERTY(meta = (BindWidget))
		class UButton* confirm;

	UPROPERTY(VisibleAnywhere) class APlayerCharacter* currentCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TEnumAsByte<AvailableCharacter> currentCharacterChosen; //setear imagen a partir de este atributo
	UPROPERTY(VisibleAnywhere) TArray<class APlayerCharacter*> characters;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int index = 0; // indice del personaje activo de la lista

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
		TSubclassOf<class UWInventory> InventoryWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UWInventory* InventoryWidget;

	UPROPERTY(VisibleAnywhere) class AUserHUD* hud;

	FTimerHandle timer;

	UFUNCTION(BlueprintImplementableEvent) void SetImagesBP(bool moveRight);
	UFUNCTION(BlueprintImplementableEvent) void SetInitialImage();
	UFUNCTION() void OnClickButtonLeft();
	UFUNCTION() void OnClickButtonRight();
	UFUNCTION() void OnClickButtonConfirm();
	UFUNCTION() void OnClickButtonFood();
	UFUNCTION() void OnClickButtonMedKit();
	UFUNCTION() void OnClickButtonInventory();
	UFUNCTION() void OnClickButtonChangeEquipment();
	UFUNCTION() void SetImages(bool moveRight);
	UFUNCTION() void DisableButtons();
	UFUNCTION(BlueprintCallable) void EnableButtons();
	UFUNCTION() void SetHealthBar();
	UFUNCTION() void  SetHealthText();
	UFUNCTION() void  SetAmmoText();
	UFUNCTION() void  SetFoodText();
	UFUNCTION() void  SetMedKitText();
	UFUNCTION() void  SetInitialValues();
	UFUNCTION() void  EnableButtonFood();
	UFUNCTION() void  EnableButtonMedKit();
	UFUNCTION() void  SetHealthAfterChanginCharacter();
};
