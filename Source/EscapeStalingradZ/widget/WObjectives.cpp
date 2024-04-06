// Fill out your copyright notice in the Description page of Project Settings.


#include "WObjectives.h"
#include "Components/CheckBox.h"
#include "EscapeStalingradZ/player/PlayerC.h"
#include "EscapeStalingradZ/turn/Turn.h"


UWObjectives::UWObjectives(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWObjectives::NativeConstruct()
{

	Super::NativeConstruct();

	CheckBox_Obj->CheckedStateDelegate.BindDynamic(this, &UWObjectives::hasPrimaryObjective);
	CheckBox_Obj->SynchronizeProperties();
	CheckBox_Obj_Second->CheckedStateDelegate.BindDynamic(this, &UWObjectives::hasSecondaryObjective);
	CheckBox_Obj_Second->SynchronizeProperties();
	player = Cast<APlayerC>(GetWorld()->GetFirstPlayerController());
}

ECheckBoxState UWObjectives::hasPrimaryObjective()
{
	if (player != nullptr) {
		if (player->hasPrimaryObjective) {
			return ECheckBoxState::Checked;
		}
	}
	return ECheckBoxState::Unchecked;
}

ECheckBoxState UWObjectives::hasSecondaryObjective()
{
	if (player != nullptr) {
		if (player->hasSecondaryObjective) {
			return ECheckBoxState::Checked;
		}
	}
	return ECheckBoxState::Unchecked;
}

void UWObjectives::SetCurrentObjectiveToTurn()
{
	turn->CurrentObjective = this;
}
