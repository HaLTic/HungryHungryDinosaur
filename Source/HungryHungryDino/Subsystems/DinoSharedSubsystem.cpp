// Fill out your copyright notice in the Description page of Project Settings.


#include "DinoSharedSubsystem.h"

#include "MassCommonFragments.h"
#include "MassEntityConfigAsset.h"
#include "MassEntitySubsystem.h"
#include "Data/DinoDataTypes.h"
#include "Data/DinoDeveloperSettings.h"
#include "Data/DinoMassFragments.h"

void UDinoSharedSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	if (const UDinoDeveloperSettings* Settings = GetDefault<UDinoDeveloperSettings>())
	{
		if (Settings)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("EntityConfig: %s"), *EntityConfig->GetName()));
			EntityConfig = Settings->EntityConfig.LoadSynchronous();
		}
	}

}

void UDinoSharedSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	if (!EntityConfig) return;
	EntityTemplate = EntityConfig->GetOrCreateEntityTemplate(InWorld);
	
	if (auto* EntitySub = InWorld.GetSubsystem<UMassEntitySubsystem>())
	{
		EntityManager = &EntitySub->GetMutableEntityManager();
	}
	SpawnerSubsystem = InWorld.GetSubsystem<UMassSpawnerSubsystem>();

	StartSpawnManager();
}

void UDinoSharedSubsystem::StartSpawnManager()
{
	const UWorld& WorldContext = GetWorldRef();
	if (!IsValid(&WorldContext)) return;

	if (!WorldContext.GetTimerManager().IsTimerActive(SpawnTimerHandle))
	{
		WorldContext.GetTimerManager().SetTimer(SpawnTimerHandle, this, &UDinoSharedSubsystem::SpawnManager, 0.1f, true);
	}
}

void UDinoSharedSubsystem::SpawnManager()
{
	ProcessPendingSpawnRequests();
	SpawnEntities();
}

void UDinoSharedSubsystem::ProcessPendingSpawnRequests()
{
	if (!EntityManager || !SpawnerSubsystem) return;
	const auto* Settings = GetDefault<UDinoDeveloperSettings>();
	if (!Settings) return;

	TArray<FMassEntityHandle> SpawnedEntities;
	int32 SpawnThisTick = Settings->MaxSpawnPerTick;
	for (int i = PendingSpawns.Num() - 1; i >- 0; --i)
	{
		FDinoSpawnRequest& Request = PendingSpawns[i];
		const int32 ThisBatch = FMath::Min(Request.Count, SpawnThisTick);
		if (ThisBatch == 0 )
		{
			PendingSpawns.RemoveAtSwap(i);
			continue;
		}

		FMassEntityTemplate RequestEntityTemplate;
		switch (Request.Type)
		{
		case EDinoEntityType::Entity: RequestEntityTemplate = EntityTemplate;
			break;
		}
		TArray<FMassEntityHandle> RequestEntities;
		SpawnerSubsystem->SpawnEntities(RequestEntityTemplate, ThisBatch, RequestEntities);

		ConfigureSpawnedEntities(Request , RequestEntities);
		Request.Count -= ThisBatch;
		SpawnThisTick -= ThisBatch;

		if (Request.Count <= 0)
		{
			PendingSpawns.RemoveAtSwap(i);
		}

		if (SpawnThisTick <= 0)
		{
			return;
		}
		
	}
}

void UDinoSharedSubsystem::ConfigureSpawnedEntities(const FDinoSpawnRequest& SpawnRequest, const TArray<FMassEntityHandle>& RequestEntities) const
{
	for (const auto& Entity : RequestEntities)
	{
		if (FTransformFragment* TransformFragment = EntityManager->GetFragmentDataPtr<FTransformFragment>(Entity))
		{
			TransformFragment->GetMutableTransform().SetLocation(SpawnRequest.WorldPos);
		}
		
		switch (SpawnRequest.Type)
		{
		case EDinoEntityType::Entity:
			{
				if (FDinoEntityFragment* EntityFragment= EntityManager->GetFragmentDataPtr<FDinoEntityFragment>(Entity))
				{
					EntityFragment->TimeToLive = SpawnRequest.TimeToLive;
				}
			}
			break;
		}
	}

}

void UDinoSharedSubsystem::SpawnEntities()
{
	// Calculate Position

	// Generate Spawn Request
	FDinoSpawnRequest SpawnRequest;
	SpawnRequest.Type = EDinoEntityType::Entity;
	SpawnRequest.WorldPos = FVector::ZeroVector;
	SpawnRequest.Count = 1;
	SpawnRequest.TimeToLive = 10.f;

	EnqueueSpawnRequest(SpawnRequest);
	
}

void UDinoSharedSubsystem::EnqueueSpawnRequest(const FDinoSpawnRequest& SpawnRequest)
{
	PendingSpawns.Add(SpawnRequest);
}

void UDinoSharedSubsystem::StopSpawnManager()
{
	const auto& WorldContext = GetWorldRef();
	if (!IsValid(&WorldContext)) return;
	WorldContext.GetTimerManager().ClearTimer(SpawnTimerHandle);
}





void UDinoSharedSubsystem::Deinitialize()
{
	Super::Deinitialize();
}
