// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WSelectCharacterTurn.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWSelectCharacterTurn : public UUserWidget
{
	GENERATED_BODY()

public: 

	UWSelectCharacterTurn(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = characters) TArray<class APlayerCharacter*> characters;

	UPROPERTY(VisibleAnywhere)
		class UCharacterButton* characterButton;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UCharacterButton> characterButtonClass;

	UPROPERTY(meta = (BindWidget))
		class UScrollBox* charactersList;

	UFUNCTION()
		void SetCharactersButton(class ATurn* turn);

	UFUNCTION()
		void onClickButton();
	
};
