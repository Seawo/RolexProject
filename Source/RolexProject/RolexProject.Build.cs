// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RolexProject : ModuleRules
{
	public RolexProject(ReadOnlyTargetRules Target) : base(Target)
	{
        // Public 파일 경로 추가
        PublicIncludePaths.AddRange(new string[] { 
            "RolexProject",
            "RolexProject/LSH",
            "RolexProject/LSH/UI",
            "RolexProject/LSH/Character",
            "RolexProject/LSH/Animation",
            "RolexProject/LSH/Actor",
            "RolexProject/CSW/Public", 
            "RolexProject/jaeun/Public"
        });

        // Private 파일 경로 추가
        PrivateIncludePaths.AddRange(new string[] { 
            "RolexProject/CSW/Private",
            "RolexProject/jaeun/Private" 
        });

        // PCH 사용 설정
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // Public 종속성 추가
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
