// TPS. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TPSBlueprintFuncLib.generated.h"

class USoundClass;

UCLASS()
class TPS_API UTPSBlueprintFuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static void SetSoundClassVolume(USoundClass* SoundClass, float Volume);

	UFUNCTION(BlueprintCallable)
	static void ToggleSoundClassVolume(USoundClass* SoundClass);
};
