// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UtilitiesESZ.generated.h"

UCLASS()
class ESCAPESTALINGRADZ_API UtilitiesESZ : public UObject
{
	GENERATED_BODY()
public:
	UtilitiesESZ();
	~UtilitiesESZ();
	static FVector SnapVectorToVector(FVector vector1, FVector vector2);
};
