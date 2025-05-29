using UnrealBuildTool;

public class RingworldGameFeatures : ModuleRules
{
    public RingworldGameFeatures(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;

        bEnableNonInlinedGenCppWarnings = true;
        
        // Define GAME_EXPORT for proper API macros
        PublicDefinitions.Add("GAME_EXPORT=1");

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core", "CoreUObject", "Engine", "GameplayAbilities", "GameplayTags", "UMG", "AIModule"
        });

        PrivateDependencyModuleNames.AddRange(new[]
        {
            "InputCore", "Slate", "SlateCore"
        });
    }
}
