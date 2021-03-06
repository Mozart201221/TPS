// TPS. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TPSWeapon.h"
#include "TPSLauncherWeapon.generated.h"

class ATPSProjectile;
class USoundCue;

UCLASS()
class TPS_API ATPSLauncherWeapon : public ATPSWeapon
{
	GENERATED_BODY()
	
public:
	virtual void StartFire() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ATPSProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* NoAmmoSound;

	virtual void MakeShot() override;
};
