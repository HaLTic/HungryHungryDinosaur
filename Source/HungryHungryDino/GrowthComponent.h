#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrowthComponent.generated.h"

class AHungryHungryDinoCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UGrowthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGrowthComponent();

protected:
	virtual void BeginPlay() override;

public:
	// grow the dino by raw units (e.g. 1.0 per food item)
	UFUNCTION(BlueprintCallable, Category="Growth")
	void AddGrowth(float Amount);

	UFUNCTION(BlueprintPure, Category="Growth")
	float GetCurrentGrowth() const { return CurrentGrowth; }

private:
	UPROPERTY(VisibleAnywhere, Category="Growth")
	float CurrentGrowth = 0.0f;

	UPROPERTY()
	TObjectPtr<AHungryHungryDinoCharacter> OwnerRef = nullptr;

	// scale added per growth unit
	UPROPERTY(EditAnywhere, Category="Growth", meta=(ClampMin="0.001"))
	float GrowthPerUnit = 0.1f;

	// dino can't shrink below this
	UPROPERTY(EditAnywhere, Category="Growth", meta=(ClampMin="0.1"))
	float MinScale = 1.0f;

	// dino can't grow beyond this
	UPROPERTY(EditAnywhere, Category="Growth", meta=(ClampMin="1.0"))
	float MaxScale = 5.0f;
};
