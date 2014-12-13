using UnrealBuildTool;

public class UE4_TurnBased : ModuleRules
{
    public UE4_TurnBased ( TargetInfo Target )
    {
	PublicDependencyModuleNames.AddRange( new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule" } );
	PrivateDependencyModuleNames.AddRange( new string[] { "RenderCore" } );
    }
}
