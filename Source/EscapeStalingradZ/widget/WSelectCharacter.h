// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WSelectCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWSelectCharacter : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UWSelectCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonLeft;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonRight;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonConfirm;

	UPROPERTY(VisibleAnywhere) class APlayerCharacter* character;

	UPROPERTY(VisibleAnywhere) class AUserHUD* hud;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int index = 0; //0 = becker, 1 = dasha, 2 = katyusha y 3 = til

	UFUNCTION(BlueprintImplementableEvent) void SetImagesBP(bool moveRight);
	UFUNCTION() void OnClickButtonLeft();
	UFUNCTION() void OnClickButtonRight();
	UFUNCTION() void OnClickButtonConfirm();
	UFUNCTION() void SetImages(bool moveRight);
	UFUNCTION() void DisableButtons();
	UFUNCTION(BlueprintCallable) void EnableButtons();

};
