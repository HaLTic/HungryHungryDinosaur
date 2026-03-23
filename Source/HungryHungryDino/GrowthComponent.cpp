#include "GrowthComponent.h"
#include "HungryHungryDinoCharacter.h"

UGrowthComponent::UGrowthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGrowthComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerRef = Cast<AHungryHungryDinoCharacter>(GetOwner());
	if (!OwnerRef)
	{
		UE_LOG(LogTemp, Error,
			TEXT("UGrowthComponent: owner '%s' is not a dino character, growth won't work"),
			*GetNameSafe(GetOwner()));
	}
}

void UGrowthComponent::AddGrowth(float Amount)
{
	CurrentGrowth += Amount;

	const float NewScale = FMath::Clamp(
		MinScale + (CurrentGrowth * GrowthPerUnit),
		MinScale,
		MaxScale
	);

	if (OwnerRef)
	{
		OwnerRef->SetScale(NewScale);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UGrowthComponent: owner ref is null, can't scale"));
	}
}
