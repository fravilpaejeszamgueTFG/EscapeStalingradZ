// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WSelectMovementType.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWSelectMovementType : public UUserWidget
{
	GENERATED_BODY()

public:

	UWSelectMovementType(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		class UBoton* buttonRunning;
	UPROPERTY(meta = (BindWidget))
		class UBoton* buttonWalking;
	UPROPERTY(meta = (BindWidget))
		class UBoton* buttonStationary;

	UPROPERTY(VisibleAnywhere)
		class UWMovimiento* actionWidget;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UWMovimiento> actionWidgetClass;

	UPROPERTY(VisibleAnywhere)
		class APlayerCharacter* character;

	UPROPERTY(VisibleAnywhere)
		class ATurn* turn;

	UFUNCTION() void OnClickRunning();
	UFUNCTION() void OnClickWalking();
	UFUNCTION() void OnClickStatic();
	UFUNCTION() void CreateOrSetVisibleActionWidget(enum MovementType type);
	
};
