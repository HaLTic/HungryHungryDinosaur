// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTemplate.h"
#include "MassSpawnerSubsystem.h"
#include "Subsystems/WorldSubsystem.h"
#include "DinoSharedSubsystem.generated.h"

class UMassSpawnerSubsystem;
struct FDinoSpawnRequest;
class UMassEntityConfigAsset;
/**
 * 
 */
UCLASS()
class HUNGRYHUNGRYDINO_API UDinoSharedSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()


public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void EnqueueSpawnRequest(const FDinoSpawnRequest& SpawnRequest);

	void StartSpawnManager();
	void StopSpawnManager();

protected:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	void SpawnManager();
	void ProcessPendingSpawnRequests();
	void ConfigureSpawnedEntities(const FDinoSpawnRequest& SpawnRequest, const TArray<FMassEntityHandle>& RequestEntities) const;
	void SpawnEntities();
	
	UPROPERTY()
	UMassEntityConfigAsset* EntityConfig;

	UPROPERTY()
	TArray<FDinoSpawnRequest> PendingSpawns;

	UPROPERTY()
	UMassSpawnerSubsystem* SpawnerSubsystem;

private:

	FTimerHandle SpawnTimerHandle;
	FMassEntityManager* EntityManager;
	FMassEntityTemplate EntityTemplate;
};

