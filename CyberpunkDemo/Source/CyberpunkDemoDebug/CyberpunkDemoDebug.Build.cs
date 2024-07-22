using UnrealBuildTool;
 
public class CyberpunkDemoDebug: ModuleRules
{
    public CyberpunkDemoDebug(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "GameplayDebugger", "CyberpunkDemo" });
    }
}