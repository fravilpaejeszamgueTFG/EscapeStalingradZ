// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilitiesESZ.h"

UtilitiesESZ::UtilitiesESZ()
{
}

UtilitiesESZ::~UtilitiesESZ()
{
}

FVector UtilitiesESZ::SnapVectorToVector(FVector vector1, FVector vector2)
{
	return FVector(FMath::GridSnap(vector1.X,vector2.X), FMath::GridSnap(vector1.Y, vector2.Y),0);
}
