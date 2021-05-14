// TPS. All Rights Reserved


#include "Weapon/TPSLauncherWeapon.h"
#include "Weapon/TPSProjectile.h"

void ATPSLauncherWeapon::StartFire()
{
	MakeShot();
}

void ATPSLauncherWeapon::MakeShot()
{
	if (!GetWorld() || IsAmmoEmpty()) return;

	FVector TraseStart, TraceEnd;
	if (!GetTraceData(TraseStart, TraceEnd)) return;

	FHitResult HitResult;
	MakeHit(HitResult, TraseStart, TraceEnd);

	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

	const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
	ATPSProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATPSProjectile>(ProjectileClass, SpawnTransform);
	if (Projectile)
	{
		Projectile->SetShotDirection(Direction);
		Projectile->SetOwner(GetOwner());
		Projectile->FinishSpawning(SpawnTransform);
	}
	DecreaseAmmo();
	SpawnMuzzleFX();
}