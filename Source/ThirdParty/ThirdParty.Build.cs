using System.IO;
using UnrealBuildTool;

public class ThirdParty : ModuleRules
{
    public ThirdParty(ReadOnlyTargetRules Target) : base(Target)
    {
        // PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );

        AddTinyXML2(Target);
    }

    void AddTinyXML2(ReadOnlyTargetRules Target)
    {
        string libSourcePath = Path.Combine(ModuleDirectory, "TinyXML2");
        PublicIncludePaths.Add(Path.Combine(libSourcePath, "include"));

        PublicAdditionalLibraries.Add(Path.Combine(libSourcePath, "lib", "release", "tinyxml2.lib"));
        
    }
}