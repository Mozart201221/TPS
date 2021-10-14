// TPS. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPSCoreTypes.h"
#include "TPSPlayerController.generated.h"

class UTPSRespawnComponent;

UCLASS()
class TPS_API ATPSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATPSPlayerController();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTPSRespawnComponent* RespawnComponent;

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;

private:
	void OnPauseGame();
	void OnMatchStateChanged(ETPSMatchState State);
	void OnMuteSound();
};
