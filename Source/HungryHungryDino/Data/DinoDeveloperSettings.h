// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "DinoDeveloperSettings.generated.h"

class UMassEntityConfigAsset;
UCLASS(Config = Game, DefaultConfig, meta= (DisplayName = "Dino Mass Settings"))
class HUNGRYHUNGRYDINO_API UDinoDeveloperSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()
	
public:
	UDinoDeveloperSettings();

	UPROPERTY(config, EditAnywhere, Category="System Settings")
	TSoftObjectPtr<UMassEntityConfigAsset> EntityConfig;

	UPROPERTY(EditDefaultsOnly, Config, Category="System Settings")
	int32 MaxSpawnPerTick = 64;
};
