// Fill out your copyright notice in the Description page of Project Settings.


#include "DinoEntitySpawnProcessor.h"

#include "MassAIBehaviorTypes.h"
#include "MassExecutionContext.h"
#include "Data/DinoMassFragments.h"
#include "Subsystems/DinoSharedSubsystem.h"

UDinoEntitySpawnProcessor::UDinoEntitySpawnProcessor() : EntityQuery(*this)
{
	bAutoRegisterWithProcessingPhases = true;
	ProcessingPhase = EMassProcessingPhase::PrePhysics;
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Tasks;
	
}

void UDinoEntitySpawnProcessor::ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager)
{
	Super::ConfigureQueries(EntityManager);

	EntityQuery.AddRequirement<FDinoEntityFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddSubsystemRequirement<UDinoSharedSubsystem>(EMassFragmentAccess::ReadWrite);

	ProcessorRequirements.AddSubsystemRequirement<UDinoSharedSubsystem>(EMassFragmentAccess::ReadWrite);
}

void UDinoEntitySpawnProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{

	EntityQuery.ForEachEntityChunk(Context, [this](FMassExecutionContext& SubContext)
	{
		const TArrayView<FDinoEntityFragment> EntityFragments = SubContext.GetMutableFragmentView<FDinoEntityFragment>();

		for (FMassExecutionContext::FEntityIterator EntityIt = SubContext.CreateEntityIterator(); EntityIt; ++EntityIt)
		{
			const FMassEntityHandle Entity = SubContext.GetEntity(EntityIt);
			FDinoEntityFragment& EntityFragment = EntityFragments[EntityIt];

			EntityFragment.TimeToLive -= SubContext.GetDeltaTimeSeconds();
			if (EntityFragment.TimeToLive <= 0)
			{
				// Destroy Entity
				SubContext.Defer().DestroyEntity(Entity);
			}
		}
	});
}
