// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WPlayerInfo.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWPlayerInfo : public UUserWidget
{
	GENERATED_BODY()

public:

	UWPlayerInfo(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* character_image;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* MPBar;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* health;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MP;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* KillNumber;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AmmoNumber;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* FoodNumber;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MedKitNumber;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* ReadyWeapon_Image;
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
		class UImage* SecondaryWeapon_Image;
	UPROPERTY(meta = (BindWidget))
		class UOverlay* SecondaryWeapon_Overlay;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* ButtonReadyWeapon;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* ButtonSecondaryWeapon;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		class UWActions* actionWidget;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		class UWObjectives* objectiveWidget;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		class UWOptions* optionsWidget;

	UPROPERTY(VisibleAnywhere)
		class ATurn* turn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character) class APlayerCharacter* character;

	UFUNCTION(BlueprintImplementableEvent) void SetCharacterImage();
	UFUNCTION(BlueprintImplementableEvent) void SetReadyWImage();
	UFUNCTION(BlueprintImplementableEvent) void SetSecondaryWImage();
	UFUNCTION() float SetHealthBar();
	UFUNCTION() float SetMPBar();
	UFUNCTION() FText  SetHealthText();
	UFUNCTION() FText  SetMPText();
	UFUNCTION() FText  SetKillNumberText();
	UFUNCTION() FText  SetAmmoNumberText();
	UFUNCTION() FText  SetFoodNumberText();
	UFUNCTION() FText  SetMedKitNumberText();
	UFUNCTION() ESlateVisibility  SetVisibilitySecondaryWeapon();
	UFUNCTION() void  UpdateImages();
	UFUNCTION() void SetVisibleActionWidget();
	UFUNCTION() void SetVisibleOptionsWidget();
	UFUNCTION() void SetVisibleObjectivesWidget();
	UFUNCTION() void OnClickReadyWeapon();
	UFUNCTION() void OnClickSecondaryWeapon();
	UFUNCTION(BlueprintImplementableEvent) void SetUseReadyWeaponColor();
	UFUNCTION(BlueprintImplementableEvent) void SetUseSecondaryWeaponColor();

	UFUNCTION()
		void HidePlayerInfoDuringCombat(class AZombie* zombie);
	UFUNCTION()
		void UnhidePlayerInfoDuringCombat();

};
