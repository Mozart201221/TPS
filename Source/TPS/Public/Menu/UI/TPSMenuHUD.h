// TPS. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TPSMenuHUD.generated.h"


UCLASS()
class TPS_API ATPSMenuHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MenuWidgetClass;
};
