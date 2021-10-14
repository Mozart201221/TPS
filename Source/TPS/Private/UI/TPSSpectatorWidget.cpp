// TPS. All Rights Reserved


#include "UI/TPSSpectatorWidget.h"
#include "TPSUtils.h"
#include "Components/TPSRespawnComponent.h"

bool UTPSSpectatorWidget::GetRespawnTime(int32& CountDownTime) const
{
	const auto RespawnComponent = TPSUtils::GetTPSPlayerComponent<UTPSRespawnComponent>(GetOwningPlayer());
	if (!RespawnComponent || !RespawnComponent->IsRespawnInProgress()) return false;

	CountDownTime = RespawnComponent->GetRespawnCountDown();
	return true;
} 