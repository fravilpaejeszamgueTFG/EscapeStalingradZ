// Fill out your copyright notice in the Description page of Project Settings.


#include "UserHUD.h"
#include "WPlayerInfo.h"
#include "EscapeStalingradZ/player/PlayerC.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

AUserHUD::AUserHUD() 
{

}

void AUserHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AUserHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerC* player = Cast<APlayerC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (player != nullptr) {
		controller = player;
	}
}

void AUserHUD::CreateOrSetPlayerInfo()
{
	if (PlayerInfoWidgetClass) {
		character = controller->playerchara;
		if (PlayerInfoWidget != nullptr) {
			PlayerInfoWidget->character = controller->playerchara;
			PlayerInfoWidget->turn = turn;
			PlayerInfoWidget->UpdateImages();
			PlayerInfoWidget->SetVisibility(ESlateVisibility::Visible);
			PlayerInfoWidget->CreateOrSetVisibleActionWidget();
		}
		else {
			PlayerInfoWidget = CreateWidget<UWPlayerInfo>(GetWorld(), PlayerInfoWidgetClass);
			if (PlayerInfoWidget != nullptr) {
				PlayerInfoWidget->character = controller->playerchara;
				PlayerInfoWidget->turn = turn;
				PlayerInfoWidget->UpdateImages();
				PlayerInfoWidget->AddToViewport();
				PlayerInfoWidget->CreateOrSetVisibleActionWidget();
			}
		}
	}
}

void AUserHUD::HidePlayerInfo()
{
	if (PlayerInfoWidget != nullptr) {
		PlayerInfoWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
