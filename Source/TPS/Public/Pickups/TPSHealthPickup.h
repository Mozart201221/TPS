// TPS. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Pickups/TPSPickup.h"
#include "TPSHealthPickup.generated.h"

UCLASS()
class TPS_API ATPSHealthPickup : public ATPSPickup
{
	GENERATED_BODY()
	
private:
	virtual bool GivePickupTo(APawn* PlayerPawn) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "10.0"))
	float HealthAmount = 100.0f;
};
