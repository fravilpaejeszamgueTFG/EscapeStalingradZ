// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WZombieInfo.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPESTALINGRADZ_API UWZombieInfo : public UUserWidget
{
	GENERATED_BODY()

public:

	UWZombieInfo(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* stunImage;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* healthBar;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Zombie) class AZombie* zombie;

	UFUNCTION() float SetHealthBar();
	UFUNCTION() FText  SetHealthText();
	UFUNCTION() ESlateVisibility  SetVisibilityStunImage();

};
