// TPS. All Rights Reserved


#include "UI/TPSPlayerHUDWidget.h"
#include "Components/TPSHealthComponent.h"
#include "Components/TPSWeaponComponent.h"
#include "TPSUtils.h"
#include "Components/ProgressBar.h"
#include "Player/TPSPlayerState.h"

void  UTPSPlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GetOwningPlayer())
	{
		GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UTPSPlayerHUDWidget::OnNewPawn);
		OnNewPawn(GetOwningPlayerPawn());
	}
}

void UTPSPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
	const auto HealthComponent = TPSUtils::GetTPSPlayerComponent<UTPSHealthComponent>(NewPawn);
	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddUObject(this, &UTPSPlayerHUDWidget::OnHealthChanged);
	}
	UpdateHealthBar();
}


void UTPSPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta)
{
	if (HealthDelta < 0.0f)
	{
		OnTakeDamage();

		if (!IsAnimationPlaying(DamageAnimation))
		{
			PlayAnimation(DamageAnimation);
		}
	}

	UpdateHealthBar();
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

void UTPSPlayerHUDWidget::UpdateHealthBar()
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetFillColorAndOpacity(GetHealthPercent() > PercentColorThreshold ? GoodColor : BadColor);
	}
}

FString UTPSPlayerHUDWidget::FormatBullets(int32 BulletsNum) const
{
	const int32 MaxLen = 3;
	const TCHAR PrefixSymbol = '0';

	auto BulletStr = FString::FromInt(BulletsNum);
	const auto SybolsNumToAdd = MaxLen - BulletStr.Len();

	if (SybolsNumToAdd > 0)
	{
		BulletStr = FString::ChrN(SybolsNumToAdd, PrefixSymbol).Append(BulletStr);
	}

	return BulletStr;
}

int32 UTPSPlayerHUDWidget::GetKillsNum() const
{
	const auto Controller = GetOwningPlayer();
	if (!Controller) return 0;

	const auto PlayerState = Cast<ATPSPlayerState>(Controller->PlayerState);
	return PlayerState ? PlayerState->GetKillsNum() : 0;
}