// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AnimatedTextAttack.generated.h"

UCLASS()
class ESCAPESTALINGRADZ_API AAnimatedTextAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAnimatedTextAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FText animatedText;
	
	UFUNCTION() void SetAnimationText(FText text);
	UFUNCTION(BlueprintImplementableEvent) void SetAnimation();

};
