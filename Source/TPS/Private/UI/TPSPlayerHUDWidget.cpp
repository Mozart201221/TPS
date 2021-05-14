// TPS. All Rights Reserved


#include "UI/TPSPlayerHUDWidget.h"
#include "Components/TPSHealthComponent.h"
#include "Components/TPSWeaponComponent.h"
#include "TPSUtils.h"

bool UTPSPlayerHUDWidget::Initialize()
{
	const auto HealthComponent = TPSUtils::GetTPSPlayerComponent<UTPSHealthComponent>(GetOwningPlayerPawn());
	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddUObject(this, &UTPSPlayerHUDWidget::OnHealthChanged);
	}

	return Super::Initialize();
}


void UTPSPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta)
{
	if (HealthDelta < 0.0f)
	{
		OnTakeDamage();
	}
}


float UTPSPlayerHUDWidget::GetHealthPercent() const
{
	const auto HealthComponent = TPSUtils::GetTPSPlayerComponent<UTPSHealthComponent>(GetOwningPlayerPawn());
	if(!HealthComponent) return 0.0f;

	return HealthComponent->GetHealthPercent();
}

bool UTPSPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	const auto WeaponComponent = TPSUtils::GetTPSPlayerComponent<UTPSWeaponComponent>(GetOwningPlayerPawn());
	if (!WeaponComponent) return false;

	return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

bool UTPSPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	const auto WeaponComponent = TPSUtils::GetTPSPlayerComponent<UTPSWeaponComponent>(GetOwningPlayerPawn());
	if (!WeaponComponent) return false;

	return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

bool UTPSPlayerHUDWidget::IsPlayerAlive() const
{
	const auto HealthComponent = TPSUtils::GetTPSPlayerComponent<UTPSHealthComponent>(GetOwningPlayerPawn());
	return HealthComponent && !HealthComponent->IsDead();
}

bool UTPSPlayerHUDWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == NAME_Spectating;
}

