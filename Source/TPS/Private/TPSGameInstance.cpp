// TPS. All Rights Reserved


#include "TPSGameInstance.h"
#include "Sound/TPSBlueprintFuncLib.h"

void UTPSGameInstance::ToggleVolume()
{
	UTPSBlueprintFuncLib::ToggleSoundClassVolume(MasterSoundClass);
}