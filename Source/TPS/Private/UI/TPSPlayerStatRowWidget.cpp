// TPS. All Rights Reserved


#include "UI/TPSPlayerStatRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UTPSPlayerStatRowWidget::SetPlayerName(const FText& Text)
{
	if (!PlayerNameTextBlock) return;
	PlayerNameTextBlock->SetText(Text);
}
void UTPSPlayerStatRowWidget::SetKills(const FText& Text) 
{
	if (!KillsTextBlock) return;
	KillsTextBlock->SetText(Text);
}
void UTPSPlayerStatRowWidget::SetDeaths(const FText& Text)
{
	if (!DeathsTextBlock) return;
	DeathsTextBlock->SetText(Text);
}
void UTPSPlayerStatRowWidget::SetTeam(const FText& Text)
{
	if (!TeamTextBlock) return;
	TeamTextBlock->SetText(Text);
}
void UTPSPlayerStatRowWidget::SetPlayerIndicatorVisibility(bool Visible)
{
	if (!PlayerIndicatorImage) return;
	PlayerIndicatorImage->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UTPSPlayerStatRowWidget::SetTeamColor(const FLinearColor& Color)
{
	if (!TeamImage) return;
	TeamImage->SetColorAndOpacity(Color);
}
