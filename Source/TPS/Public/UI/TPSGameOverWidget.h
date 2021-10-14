// TPS. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/TPSBaseWidget.h"
#include "TPSCoreTypes.h"
#include "TPSGameOverWidget.generated.h"

class UVerticalBox;
class UButton;

UCLASS()
class TPS_API UTPSGameOverWidget : public UTPSBaseWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PlayerStatBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerStatRowWidgetClass;

	UPROPERTY(meta = (BindWidget))
	UButton* ResetLevelButton;

	virtual void NativeOnInitialized() override;

private:
	void OnMatchStateChanged(ETPSMatchState State);
	void UpdatePlayersStat();

	UFUNCTION()
	void OnResetLevel();
};
