// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FPSExample : ModuleRules
{
    public FPSExample(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]{ "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });

        PublicIncludePaths.AddRange(new string[]
            {
            "FPSExample",
            "FPSExample/Public/Core",
            "FPSExample/Public/Utils",
            "FPSExample/Public/Actors",
            "FPSExample/Public/Components",
            "FPSExample/Public/Managers",
            "FPSExample/Public/Characters"
            });
    }
}