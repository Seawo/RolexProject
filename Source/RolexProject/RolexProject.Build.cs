// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RolexProject : ModuleRules
{
	public RolexProject(ReadOnlyTargetRules Target) : base(Target)
	{
        PublicIncludePaths.AddRange(new string[] { "RolexProject" });

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
