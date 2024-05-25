// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimatedTextAttack.h"

// Sets default values
AAnimatedTextAttack::AAnimatedTextAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAnimatedTextAttack::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAnimatedTextAttack::SetAnimationText(FText text)
{
	animatedText = text;
	SetAnimation();
}

