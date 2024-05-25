// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Styling/SlateTypes.h"
#include "WObjectives.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWObjectives : public UUserWidget
{
	GENERATED_BODY()

public:

	UWObjectives(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UCheckBox* CheckBox_Obj;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UCheckBox* CheckBox_Obj_Second;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<FVector2D> positionsOfX;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) class ATurn* turn;
	UPROPERTY(VisibleAnywhere) class APlayerC* player;

	UFUNCTION(BlueprintImplementableEvent) void SetNumberRoundInImage(int number);
	UFUNCTION() ECheckBoxState hasPrimaryObjective();
	UFUNCTION() ECheckBoxState hasSecondaryObjective();
	UFUNCTION(BlueprintCallable) void SetCurrentObjectiveToTurn();
	
};
