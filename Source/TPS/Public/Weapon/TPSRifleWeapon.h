// TPS. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TPSWeapon.h"
#include "TPSRifleWeapon.generated.h"

class UTPSWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UAudioComponent;

UCLASS()
class TPS_API ATPSRifleWeapon : public ATPSWeapon
{
	GENERATED_BODY()


public:
	ATPSRifleWeapon();

	virtual void StartFire() override;
	virtual void StopFire() override;
	virtual void Zoom(bool Enabled) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TimeBetweenShots = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float BulletsSpread = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float DamageAmount = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UNiagaraSystem* TraceFX;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	FString TraceTargetName = "TraceTarget";

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UTPSWeaponFXComponent* WeaponFXComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float FOVZoomAngle = 50.0f;

	virtual void BeginPlay() override;
	virtual void MakeShot() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

private:
	FTimerHandle ShootTimerHandle;

	UPROPERTY()
	UNiagaraComponent* MuzzleFXComponent;

	UPROPERTY()
	UAudioComponent* FireAudioComponent;

	void MakeDamage(const FHitResult& HitResult);

	void InitFX();
	void SetFXActive(bool IsActive);
	void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);

	AController* GetController() const;

	float DefaultCameraFOV = 90.0f;
};
