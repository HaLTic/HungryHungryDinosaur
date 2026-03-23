#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EatingComponent.generated.h"

class UGrowthComponent;
class UMeshComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UEatingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEatingComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// how close to auto-eat (scales with dino size)
	UPROPERTY(EditAnywhere, Category="Eating", meta=(ClampMin="50.0"))
	float EatRadius = 200.0f;

	// how far away edible objects start glowing (scales with dino size)
	UPROPERTY(EditAnywhere, Category="Eating", meta=(ClampMin="100.0"))
	float HighlightRadius = 800.0f;

	// growth per object eaten
	UPROPERTY(EditAnywhere, Category="Eating", meta=(ClampMin="0.01"))
	float GrowthPerEat = 1.0f;

	// seconds between eats
	UPROPERTY(EditAnywhere, Category="Eating", meta=(ClampMin="0.0"))
	float EatCooldown = 0.15f;

	// translucent overlay applied to edible objects
	UPROPERTY(EditAnywhere, Category="Eating")
	TObjectPtr<UMaterialInterface> HighlightMaterial = nullptr;

	float CooldownRemaining = 0.0f;

	UPROPERTY()
	TObjectPtr<UGrowthComponent> GrowthComp = nullptr;

	UPROPERTY()
	TObjectPtr<AActor> OwnerActor = nullptr;

	UPROPERTY()
	TSet<TObjectPtr<AActor>> HighlightedActors;

	bool IsSmallerThanOwner(AActor* TargetActor) const;
	AActor* FindClosestEdible() const;
	TArray<AActor*> FindAllEdibleInRadius(float Radius) const;
	void ApplyHighlight(AActor* Actor);
	void RemoveHighlight(AActor* Actor);
	void UpdateHighlights();
};
