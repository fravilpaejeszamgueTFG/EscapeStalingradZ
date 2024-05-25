// Fill out your copyright notice in the Description page of Project Settings.


#include "WSelectCharacterTurn.h"
#include "CharacterButton.h"
#include "Components/ScrollBox.h"
#include "EscapeStalingradZ/character/PlayerCharacter.h"

UWSelectCharacterTurn::UWSelectCharacterTurn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UWSelectCharacterTurn::NativeConstruct()
{
	Super::NativeConstruct();

}

void UWSelectCharacterTurn::SetCharactersButton(ATurn* turn)
{
	charactersList->ClearChildren();
	for (APlayerCharacter* chara : characters) {
		if (characterButtonClass) {
			characterButton = CreateWidget<UCharacterButton>(GetWorld(), characterButtonClass);
			if (characterButton != nullptr) {
				characterButton->character = chara;
				characterButton->turn = turn;
				characterButton->SetButtonStyle(chara->characterChosen);
				characterButton->selectCharacter = this;
				charactersList->AddChild(characterButton);
			}
		}
	}
}

void UWSelectCharacterTurn::onClickButton()
{
	SetVisibility(ESlateVisibility::Hidden);
}
