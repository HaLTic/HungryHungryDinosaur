#include "EatingComponent.h"
#include "GrowthComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/MeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UEatingComponent::UEatingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEatingComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = GetOwner();
	if (OwnerActor)
	{
		GrowthComp = OwnerActor->FindComponentByClass<UGrowthComponent>();
	}

	if (!GrowthComp)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("UEatingComponent: No GrowthComponent found on owner '%s'."),
			*GetNameSafe(OwnerActor));
	}

	if (!HighlightMaterial)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("UEatingComponent: No HighlightMaterial assigned — edible objects won't glow."));
	}
}

void UEatingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateHighlights();

	if (CooldownRemaining > 0.0f)
	{
		CooldownRemaining -= DeltaTime;
		return;
	}

	AActor* Target = FindClosestEdible();
	if (!Target) return;

	UE_LOG(LogTemp, Log, TEXT("UEatingComponent: Eating '%s'"), *Target->GetName());

	HighlightedActors.Remove(Target);
	Target->Destroy();

	if (GrowthComp)
	{
		GrowthComp->AddGrowth(GrowthPerEat);
	}

	CooldownRemaining = EatCooldown;
}

bool UEatingComponent::IsSmallerThanOwner(AActor* TargetActor) const
{
	if (!TargetActor || !OwnerActor) return false;

	// compare capsule half-height (dino) vs bounding box (target)
	float OwnerSize = 0.0f;
	if (const ACharacter* CharOwner = Cast<ACharacter>(OwnerActor))
	{
		if (const UCapsuleComponent* Capsule = CharOwner->GetCapsuleComponent())
		{
			OwnerSize = Capsule->GetScaledCapsuleHalfHeight();
		}
	}

	FVector TargetOrigin, TargetExtent;
	TargetActor->GetActorBounds(false, TargetOrigin, TargetExtent);
	const float TargetSize = TargetExtent.GetMax();

	return TargetSize < OwnerSize;
}

TArray<AActor*> UEatingComponent::FindAllEdibleInRadius(float Radius) const
{
	TArray<AActor*> Result;
	if (!OwnerActor) return Result;

	const FVector Origin = OwnerActor->GetActorLocation();
	const float ScaledRadius = Radius * OwnerActor->GetActorScale3D().GetMax();

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(OwnerActor);

	TArray<AActor*> OverlapResults;
	UKismetSystemLibrary::SphereOverlapActors(
		OwnerActor->GetWorld(),
		Origin,
		ScaledRadius,
		ObjectTypes,
		nullptr,
		IgnoredActors,
		OverlapResults
	);

	for (AActor* Candidate : OverlapResults)
	{
		if (!Candidate || Candidate == OwnerActor) continue;
		if (!IsSmallerThanOwner(Candidate)) continue;
		Result.Add(Candidate);
	}

	return Result;
}

AActor* UEatingComponent::FindClosestEdible() const
{
	if (!OwnerActor) return nullptr;

	const FVector Origin = OwnerActor->GetActorLocation();
	TArray<AActor*> Edibles = FindAllEdibleInRadius(EatRadius);

	AActor* BestTarget = nullptr;
	float BestDistSq = MAX_FLT;

	for (AActor* Candidate : Edibles)
	{
		const float DistSq = FVector::DistSquared(Origin, Candidate->GetActorLocation());
		if (DistSq < BestDistSq)
		{
			BestDistSq = DistSq;
			BestTarget = Candidate;
		}
	}

	return BestTarget;
}

void UEatingComponent::ApplyHighlight(AActor* Actor)
{
	if (!Actor || !HighlightMaterial) return;

	TArray<UMeshComponent*> MeshComps;
	Actor->GetComponents<UMeshComponent>(MeshComps);

	for (UMeshComponent* Mesh : MeshComps)
	{
		if (Mesh)
		{
			Mesh->SetOverlayMaterial(HighlightMaterial);
		}
	}
}

void UEatingComponent::RemoveHighlight(AActor* Actor)
{
	if (!Actor) return;

	TArray<UMeshComponent*> MeshComps;
	Actor->GetComponents<UMeshComponent>(MeshComps);

	for (UMeshComponent* Mesh : MeshComps)
	{
		if (Mesh)
		{
			Mesh->SetOverlayMaterial(nullptr);
		}
	}
}

void UEatingComponent::UpdateHighlights()
{
	TArray<AActor*> CurrentEdibles = FindAllEdibleInRadius(HighlightRadius);
	TSet<TObjectPtr<AActor>> NewHighlightSet;

	for (AActor* Actor : CurrentEdibles)
	{
		NewHighlightSet.Add(Actor);

		if (!HighlightedActors.Contains(Actor))
		{
			ApplyHighlight(Actor);
		}
	}

	// unhighlight anything no longer edible or out of range
	for (auto It = HighlightedActors.CreateIterator(); It; ++It)
	{
		AActor* OldActor = *It;
		if (OldActor && !NewHighlightSet.Contains(OldActor))
		{
			RemoveHighlight(OldActor);
		}
	}

	HighlightedActors = NewHighlightSet;
}
