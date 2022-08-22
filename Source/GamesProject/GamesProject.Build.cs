// Shoot Game

using UnrealBuildTool;

public class GamesProject : ModuleRules
{
    public GamesProject(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "Niagara",   /*подключили дл€ визуальных эффектов*/
            "PhysicsCore"   /*подключили дл€ симул€ции физических материалов*/
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[] 
        {

            "GamesProject/Public/Player",
            "GamesProject/Public/Components",
            "GamesProject/Public/DevContent",
            "GamesProject/Public/Weapon",
            "GamesProject/Public/UI",
            "GamesProject/Public/Animations",
            "GamesProject/Public/Puckups",
            "GamesProject/Public/Weapon/Components"

        }); //указали путь до персонажа, фикс бага


        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
