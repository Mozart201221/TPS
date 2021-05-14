// TPS. All Rights Reserved


#include "Player/TPSPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogTPSPlayerState, All, All);

void ATPSPlayerState::LogInfo()
{
	UE_LOG(LogTPSPlayerState, Display, TEXT("TeamID: %i, Kills: %i, Deaths: %i"), TeamID, KillsNum, DeathsNum);
}