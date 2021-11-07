using System;

namespace UnrealBuildTool.Rules
{
	public class DirectInput : ModuleRules
	{
		public DirectInput(ReadOnlyTargetRules Target) : base(Target)
		{
			PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
			
			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Engine",
					"ApplicationCore",
					"InputCore",
					"SlateCore",
					"Slate"
				}
			);

			PrivateDependencyModuleNames.AddRange(new string[] { "InputDevice" } );

			PublicAdditionalLibraries.AddRange(new string[] { "dinput8.lib", "dxguid.lib" });
		}
	}
}
