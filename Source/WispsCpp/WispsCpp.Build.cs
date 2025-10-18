// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WispsCpp : ModuleRules
{
    public WispsCpp(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // Modules that are safe for both Runtime and Editor
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
            });

        // Modules that should ONLY be included when building for the Editor
        if (Target.Type == TargetType.Editor)
        {
            PublicDependencyModuleNames.AddRange(
                new string[]
                {
                    "AssetTools",
                    "UnrealEd"
                }
            );
        }
    }
}