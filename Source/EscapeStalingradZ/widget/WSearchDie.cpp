// Fill out your copyright notice in the Description page of Project Settings.


#include "WSearchDie.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"
#include "UserHUD.h"
#include "WPlayerInfo.h"

UWSearchDie::UWSearchDie(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWSearchDie::NativeConstruct()
{
    Super::NativeConstruct();

    ButtonConfirm->OnClicked.AddDynamic(this, &UWSearchDie::OnClickButtonConfirm);
    ButtonRollAnimation->OnClicked.AddDynamic(this, &UWSearchDie::OnClickButtonRollAnimation);
    hud = Cast<AUserHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void UWSearchDie::SetDie(int numberOfDie)
{
    currentNumber = numberOfDie;
    SetHumanDieImage(currentNumber);
}

void UWSearchDie::OnClickButtonConfirm()
{
    //TO-DO implementar objetivos en cada nivel
    SetVisibility(ESlateVisibility::Hidden);
    if (hud != nullptr && hud->PlayerInfoWidget != nullptr) {
        hud->PlayerInfoWidget->UnhidePlayerInfoDuringSearch();
    }
}
