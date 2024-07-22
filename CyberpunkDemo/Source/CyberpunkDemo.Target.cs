// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CyberpunkDemoTarget : TargetRules
{
	public CyberpunkDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("CyberpunkDemo");
		if (bCompileAgainstEditor)
		{
			ExtraModuleNames.Add("CyberpunkDemoDebug");
		}
    }
}
