// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTraitBase.h"
#include "Data/DinoMassFragments.h"
#include "SimpleRandomMovementTrait.generated.h"

USTRUCT()
struct FSimpleMovementFragment : public FMassFragment
{
	GENERATED_BODY()
	FVector Target;
	
};

UCLASS()
class HUNGRYHUNGRYDINO_API USimpleRandomMovementTrait : public UMassEntityTraitBase
{
	GENERATED_BODY()

protected:
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
	
};
