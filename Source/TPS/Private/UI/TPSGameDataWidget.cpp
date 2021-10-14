// TPS. All Rights Reserved


#include "UI/TPSGameDataWidget.h"
#include "TPSGameMode.h"
#include "Player/TPSPlayerState.h"

int32 UTPSGameDataWidget::GetCurrentRoundNum() const
{
	const auto GameMode = GetTPSGameMode();
	return GameMode ? GameMode->GetCurrentRoundNum() : 0;
}

int32 UTPSGameDataWidget::GetTotalRoundsNum() const
{
	const auto GameMode = GetTPSGameMode();
	return GameMode ? GameMode->GetGameData().RoundsNum : 0;
}

int32 UTPSGameDataWidget::GetRoundSecondsRemaining() const
{
	const auto GameMode = GetTPSGameMode();
	return GameMode ? GameMode->GetRoundSecondsRemaining() : 0;
}

ATPSGameMode* UTPSGameDataWidget::GetTPSGameMode() const
{
	return GetWorld() ? Cast<ATPSGameMode>(GetWorld()->GetAuthGameMode()) : NULL;
}

ATPSPlayerState* UTPSGameDataWidget::GetTPSPlayerState() const
{
	return GetOwningPlayer() ? Cast<ATPSPlayerState>(GetOwningPlayer()->PlayerState) : NULL;
}

