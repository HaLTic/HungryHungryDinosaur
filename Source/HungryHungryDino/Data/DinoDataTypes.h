// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DinoDataTypes.generated.h"


UENUM()
enum class EDinoEntityType : uint8
{
	Entity
};

USTRUCT()
struct FDinoSpawnRequest
{
	GENERATED_BODY()
	
public:
	EDinoEntityType Type = EDinoEntityType::Entity;
	int32 Count = 0;
	FVector WorldPos = FVector::Zero();
	float TimeToLive = 10.f;
	
	
};
