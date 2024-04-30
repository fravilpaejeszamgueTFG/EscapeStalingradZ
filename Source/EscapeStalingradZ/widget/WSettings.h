// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WSettings.generated.h"


/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWSettings : public UUserWidget
{
	GENERATED_BODY()

public:

	UWSettings(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		class UButton* ButtonSpanish;
	UPROPERTY(meta = (BindWidget))
		class UButton* ButtonEnglish;
	UPROPERTY(meta = (BindWidget))
		class UButton* ButtonLeftFPS;
	UPROPERTY(meta = (BindWidget))
		class UButton* ButtonRightFPS;
	UPROPERTY(meta = (BindWidget))
		class UButton* ButtonLeftResolution;
	UPROPERTY(meta = (BindWidget))
		class UButton* ButtonRightResolution;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextFPS;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextResolution;
	UPROPERTY(meta = (BindWidget))
		class UButton* ButtonBack;

	UPROPERTY() class UGI* GI;

	UPROPERTY() TArray<FString> fpsTexts = { TEXT("30"), TEXT("60"), TEXT("144"), (NSLOCTEXT("fps", "NoLimit", "No limit")).ToString() };

	UPROPERTY(VisibleAnywhere) TArray<FText> resolutionsTexts = { NSLOCTEXT("resolutions", "Low", "Low"), NSLOCTEXT("resolutions", "Medium", "Medium"), NSLOCTEXT("resolutions", "High", "High") };

	UFUNCTION() void OnClickSpanish();
	UFUNCTION() void OnClickEnglish();
	UFUNCTION() void OnClickButtonLeftFPS();
	UFUNCTION() void OnClickButtonRightFPS();
	UFUNCTION() void OnClickButtonLeftResolution();
	UFUNCTION() void OnClickButtonRightResolution();
	UFUNCTION() void OnClickBack();
	UFUNCTION() void DisableButtonsInitial();
	UFUNCTION() void SetTextFPSGivenIndex(int index);
	UFUNCTION() void SetTextResolutionGivenIndex(int index);
};
