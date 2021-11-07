using UnrealBuildTool;
using System.Collections.Generic;

public class DummyTarget : TargetRules
{
	public DummyTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		
		DefaultBuildSettings = BuildSettingsVersion.V2;
		bUseUnityBuild = false;
		bUsePCHFiles = false;

		ExtraModuleNames.AddRange( new string[] { "Dummy" } );
	}
}
