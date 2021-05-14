// TPS. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Pickups/TPSPickup.h"
#include "TPSAmmoPickup.generated.h"

class ATPSWeapon;

UCLASS()
class TPS_API ATPSAmmoPickup : public ATPSPickup
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "10.0"))
	int32 ClipsAmount = 10; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	TSubclassOf<ATPSWeapon> WeaponType;

private:
	virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
