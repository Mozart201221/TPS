// TPS. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSRespawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_API UTPSRespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTPSRespawnComponent();

	void Respawn(int32 RespawnTime);
	int32 GetRespawnCountDown() const { return RespawnCountDown; }
	bool IsRespawnInProgress() const;

private:
	FTimerHandle RespawnTimerHandle;
	int32 RespawnCountDown = 0;

	void RespawnTimerUpdate();

};
