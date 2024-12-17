// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RolexProject : ModuleRules
{
	public RolexProject(ReadOnlyTargetRules Target) : base(Target)
	{
        // Public ���� ��� �߰�
        PublicIncludePaths.AddRange(new string[] { 
            "RolexProject", 
            "RolexProject/LSH" , 
            "RolexProject/CSW/Public", 
            "RolexProject/jaeun/Public"
        });

        // Private ���� ��� �߰�
        PrivateIncludePaths.AddRange(new string[] { 
            "RolexProject/CSW/Private",
            "RolexProject/jaeun/Private" 
        });

        // PCH ��� ����
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // Public ���Ӽ� �߰�
		PublicDependencyModuleNames.AddRange(new string[] { 
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore", 
            "EnhancedInput",
            "UMG",
            "OnlineSubsystem",
            "OnlineSubsystemSteam",
            "Niagara"
        });
	}
}
