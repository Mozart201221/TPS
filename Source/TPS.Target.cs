// TPS. All Rights Reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class TPSTarget : TargetRules
{
	public TPSTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "TPS" } );
	}
}
