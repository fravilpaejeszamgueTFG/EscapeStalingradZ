// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WSelectStartTurn.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWSelectStartTurn : public UUserWidget
{
	GENERATED_BODY()

public:

	UWSelectStartTurn(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonZombie;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonHuman;

	UPROPERTY(VisibleAnywhere) class ATurn* turn;

	UFUNCTION() void OnClickButtonHuman();
	UFUNCTION() void OnClickButtonZombie();
	
};
