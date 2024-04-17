// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EscapeStalingradZ/enum/Enumerates.h"
#include "WSelectCharacterToFree.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWSelectCharacterToFree : public UUserWidget
{
	GENERATED_BODY()

public:
	UWSelectCharacterToFree(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonLeft;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonRight;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonConfirm;

	UPROPERTY(VisibleAnywhere) class ACharacterToFree* character;

	UPROPERTY(VisibleAnywhere) class AUserHUD* hud;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TArray<TEnumAsByte<AvailableCharacter>> charactersAvailable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int index = 0; //indice de la lista de charactersAvailable

	UFUNCTION(BlueprintImplementableEvent) void SetImagesBP(bool moveRight);
	UFUNCTION() void OnClickButtonLeft();
	UFUNCTION() void OnClickButtonRight();
	UFUNCTION() void OnClickButtonConfirm();
	UFUNCTION() void SetImages(bool moveRight);
	UFUNCTION() void DisableButtons();
	UFUNCTION(BlueprintCallable) void EnableButtons();
	UFUNCTION(BlueprintImplementableEvent) void SetInitialImage();
	UFUNCTION() void SetCharactersAvailable(AvailableCharacter characterAlreadyChosen);
	
};
