// TPS. All Rights Reserved


#include "Weapon/TPSRifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "Weapon/Components/TPSWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

ATPSRifleWeapon::ATPSRifleWeapon()
{
	WeaponFXComponent = CreateDefaultSubobject<UTPSWeaponFXComponent>("WeaponFXComponent");
}


void ATPSRifleWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponFXComponent);
}

void ATPSRifleWeapon::StartFire()
{
	InitFX();
	GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &ATPSRifleWeapon::MakeShot, TimeBetweenShots, true);
	MakeShot();
}

void ATPSRifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShootTimerHandle);
	SetFXActive(false);
}

void ATPSRifleWeapon::MakeShot()
{
	// UE_LOG(LogTemp, Display, TEXT("Make shot")); 

	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}

	FVector TraseStart, TraceEnd;
	if (!GetTraceData(TraseStart, TraceEnd))
	{
		StopFire();
		return;
	}

	FHitResult HitResult;
	MakeHit(HitResult, TraseStart, TraceEnd);

	FVector TraceFXEnd = TraceEnd;
	if (HitResult.bBlockingHit)
	{
		TraceFXEnd = HitResult.ImpactPoint;
		MakeDamage(HitResult);
		WeaponFXComponent->PlayImpactFX(HitResult);
	}
	SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
	DecreaseAmmo();
}

bool ATPSRifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const auto HalfRad = FMath::DegreesToRadians(BulletsSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ATPSRifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamageActor = HitResult.GetActor();
	if (!DamageActor) return;

	FPointDamageEvent PointDamageEvent;
	PointDamageEvent.HitInfo = HitResult;
	DamageActor->TakeDamage(DamageAmount, PointDamageEvent, GetController(), this);
}

void ATPSRifleWeapon::InitFX()
{
	if (!MuzzleFXComponent)
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}
	if (!FireAudioComponent)
	{
		FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
	}

	SetFXActive(true);
}

void ATPSRifleWeapon::SetFXActive(bool IsActive)
{
	if (MuzzleFXComponent)
	{
		MuzzleFXComponent->SetPaused(!IsActive);
		MuzzleFXComponent->SetVisibility(IsActive, true);
	}

	if (FireAudioComponent)
	{
		IsActive ? FireAudioComponent->Play() : FireAudioComponent->Stop();
	}
}

void ATPSRifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{ 
	const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
	if (TraceFXComponent)
	{
		TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
	}
}

AController* ATPSRifleWeapon::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : NULL;
}

void ATPSRifleWeapon::Zoom(bool Enabled)
{
	const auto Controller = Cast<APlayerController>(GetController());
	if (!Controller || !Controller->PlayerCameraManager) return;

	if (Enabled)
	{
		DefaultCameraFOV = Controller->PlayerCameraManager->GetFOVAngle();
	}
	Controller->PlayerCameraManager->SetFOV(Enabled ? FOVZoomAngle : DefaultCameraFOV);
}
