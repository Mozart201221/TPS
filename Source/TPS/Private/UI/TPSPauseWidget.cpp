// TPS. All Rights Reserved


#include "UI/TPSPauseWidget.h"
#include "Gameframework/GameModeBase.h"
#include "Components/Button.h"

void UTPSPauseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ClearPauseButton)
	{
		ClearPauseButton->OnClicked.AddDynamic(this, &UTPSPauseWidget::OnClearPause);
	}
}

void UTPSPauseWidget::OnClearPause()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

	GetWorld()->GetAuthGameMode()->ClearPause();
}

