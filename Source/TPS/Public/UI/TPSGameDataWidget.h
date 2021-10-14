// TPS. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSGameDataWidget.generated.h"

class ATPSGameMode;
class ATPSPlayerState;

UCLASS()
class TPS_API UTPSGameDataWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetCurrentRoundNum() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetTotalRoundsNum() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetRoundSecondsRemaining() const;

private:
	ATPSGameMode* GetTPSGameMode() const;
	ATPSPlayerState* GetTPSPlayerState() const;
	
};
