// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DarkSwarm : ModuleRules
{
	public DarkSwarm(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara", "UMG" });
	}
}
