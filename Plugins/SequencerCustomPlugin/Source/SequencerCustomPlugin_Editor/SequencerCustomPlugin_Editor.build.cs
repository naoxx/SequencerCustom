using UnrealBuildTool;
 
public class SequencerCustomPlugin_Editor : ModuleRules
{
	public SequencerCustomPlugin_Editor(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UnrealEd"});
 
		//PublicIncludePaths.AddRange(new string[] {"SequencerCustomPlugin_Editor/Public"});
		//PrivateIncludePaths.AddRange(new string[] {"SequencerCustomPlugin_Editor/Private"});

		PublicDependencyModuleNames.AddRange(new string[] {
			"SequencerCustomPlugin" ,
			"MovieScene",
			"Sequencer",
			"SequencerCore",
			"SlateCore",
			"LevelSequence",
			});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"Slate",
			});
	}
}