using UnrealBuildTool;
using System.Collections.Generic;

public class DummyEditorTarget : TargetRules
{
	public DummyEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		
		DefaultBuildSettings = BuildSettingsVersion.V2;
		bUseUnityBuild = false;
		bUsePCHFiles = false;

		ExtraModuleNames.AddRange( new string[] { "Dummy" } );
	}
}
