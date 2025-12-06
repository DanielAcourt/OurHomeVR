// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SplinePlugin : ModuleRules
{
	public SplinePlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine"
			}
			);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
			}
			);
	}
}
