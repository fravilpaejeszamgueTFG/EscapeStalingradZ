// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterToFree.h"
#include "EscapeStalingradZ/widget/UserHUD.h"

// Sets default values
ACharacterToFree::ACharacterToFree()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(FName("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>("InstancedMesh");
	mesh->SetupAttachment(DefaultSceneRoot);

}

// Called when the game starts or when spawned
void ACharacterToFree::BeginPlay()
{
	Super::BeginPlay();

	AUserHUD* hud = Cast<AUserHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (hud != nullptr) {
		hud->characterToFree = this;
	}
}

