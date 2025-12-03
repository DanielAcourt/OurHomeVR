// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WispPlugin : ModuleRules
{
	public WispPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"EnhancedInput",
				"Json",
				"JsonUtilities",
				"GameplayTags",
				"UMG"
			}
			);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"SlateCore"
			}
			);

		if (Target.Type == TargetType.Editor)
        {
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "AssetTools",
                    "UnrealEd"
                }
            );
        }
	}
}
