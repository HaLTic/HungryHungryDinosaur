// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HungryHungryDino : ModuleRules
{
	public HungryHungryDino(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"HungryHungryDino",
			"HungryHungryDino/Variant_Platforming",
			"HungryHungryDino/Variant_Platforming/Animation",
			"HungryHungryDino/Variant_Combat",
			"HungryHungryDino/Variant_Combat/AI",
			"HungryHungryDino/Variant_Combat/Animation",
			"HungryHungryDino/Variant_Combat/Gameplay",
			"HungryHungryDino/Variant_Combat/Interfaces",
			"HungryHungryDino/Variant_Combat/UI",
			"HungryHungryDino/Variant_SideScrolling",
			"HungryHungryDino/Variant_SideScrolling/AI",
			"HungryHungryDino/Variant_SideScrolling/Gameplay",
			"HungryHungryDino/Variant_SideScrolling/Interfaces",
			"HungryHungryDino/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
