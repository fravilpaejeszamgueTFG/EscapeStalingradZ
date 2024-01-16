// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "CharacterButton.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UCharacterButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UCharacterButton(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
		class UBoton* buttonChara;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class APlayerCharacter* character;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class ATurn* turn;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AGrid* grid;
	UPROPERTY(VisibleAnywhere)
		class UWSelectCharacterTurn* selectCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FIntPoint tile = FIntPoint(-1,-1);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetButtonStyle(enum AvailableCharacter characterChosen);

	UFUNCTION() void OnClick();
	UFUNCTION() void OnHover();
	UFUNCTION() void OnUnHover();
	
};
