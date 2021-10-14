// TPS. All Rights Reserved


#include "UI/TPSHealthBarWidget.h"
#include "Components/ProgressBar.h"

void UTPSHealthBarWidget::SetHealthPercent(float Percent)
{
	const auto HealthBarVisibility = (Percent > PercentVisibilityThreshold || FMath::IsNearlyZero(Percent)) //
										? ESlateVisibility::Hidden
										: ESlateVisibility::Visible;

	HealthProgressBar->SetVisibility(HealthBarVisibility);

	const auto HealthBarColor = Percent > PercentColorThreshold ? GoodColor : BadColor;
	HealthProgressBar->SetFillColorAndOpacity(HealthBarColor);

	HealthProgressBar->SetPercent(Percent);
}