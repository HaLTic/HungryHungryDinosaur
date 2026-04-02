// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityElementTypes.h"
#include "DinoMassFragments.generated.h"


USTRUCT()
struct FDinoEntityFragment : public FMassFragment
{
	GENERATED_BODY()
	FVector Position;
	FVector Forward;
	float TimeToLive = 10.f;
	
};
