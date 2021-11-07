using System;

namespace UnrealBuildTool.Rules
{
	public class HapticVehicle : ModuleRules
	{
		public HapticVehicle(ReadOnlyTargetRules Target) : base(Target)
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
					"Slate",
					"PhysXVehicles"
				}
			);
		}
	}
}
