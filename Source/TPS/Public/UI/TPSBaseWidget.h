// TPS. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSBaseWidget.generated.h"

class USoundCue;

UCLASS()
class TPS_API UTPSBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Show();
	
protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ShowAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* OpenSound;
};
