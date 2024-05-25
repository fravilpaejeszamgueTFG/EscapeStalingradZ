// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EscapeStalingradZ/enum/Enumerates.h"
#include "WNewOrLoadGame.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWNewOrLoadGame : public UUserWidget
{
	GENERATED_BODY()

public:

	UWNewOrLoadGame(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonSlot1;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonSlot2;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonSlot3;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* NewGame1;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* NewGame2;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* NewGame3;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* currentLevel1;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* CurrentLevel2;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* CurrentLevel3;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UHorizontalBox* HorizontalBoxSaveGameSlot1;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UHorizontalBox* HorizontalBoxSaveGameSlot2;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UHorizontalBox* HorizontalBoxSaveGameSlot3;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* ImageSlot1;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* ImageSlot2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* ImageSlot3;

	//confirmacion cargar/empezar partida
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* confirmationText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonConfirm;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonBack;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UBorder* borderConfirm;

	UPROPERTY(BlueprintReadOnly) bool isNewGame = true;

	UPROPERTY(VisibleAnywhere) class UGI* GI;
	UPROPERTY(VisibleAnywhere) TMap<FString,class USaveGameEscape*> savedGames;
	UPROPERTY(VisibleAnywhere) FName levelToLoad;

	UPROPERTY(VisibleAnywhere, Category = package)
		TArray<class UPackage*> packages;

	UFUNCTION(BlueprintImplementableEvent) void SetCharacterImageGivenImageAndCharacterName(class UImage* imagen, AvailableCharacter characterChosen); //TO-DO
	UFUNCTION() void OnClickButtonSlot1();
	UFUNCTION() void OnClickButtonSlot2();
	UFUNCTION() void OnClickButtonSlot3();
	UFUNCTION() void OnClickButtonGivenSlot(FString slotName);
	UFUNCTION() void OnClickButtonConfirm();
	UFUNCTION() void OnClickButtonBack();
	UFUNCTION() FText GetCurrentLevelText(ScenarioName scenario);
	UFUNCTION() void SetVisibilityTextAndImages();
	UFUNCTION() void PrecacheGivenPackage(const FString& name);
	UFUNCTION() void SetVisibilityTextAndImagesByGivenParameters(FString slotName, UTextBlock* newGame, UImage* imagen, UTextBlock* currentLevel, UHorizontalBox* box);
	UFUNCTION() void SetEnableButtonsInLoadGame(FString slotName, UButton* button);
	UFUNCTION() void PrecacheMapsInLoad();
	UFUNCTION() FString GetRouteGivenScenarioName(ScenarioName scenario);
	UFUNCTION() void SetLevelToLoadGivenScenario(ScenarioName scenario);

};
