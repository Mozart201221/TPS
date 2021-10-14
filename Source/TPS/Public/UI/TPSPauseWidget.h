// TPS. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/TPSBaseWidget.h"
#include "TPSPauseWidget.generated.h"

class UButton;

UCLASS()
class TPS_API UTPSPauseWidget : public UTPSBaseWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ClearPauseButton;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnClearPause();
	
};
