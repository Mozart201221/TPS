// TPS. All Rights Reserved


#include "UI/TPSGameOverWidget.h"
#include "TPSGameMode.h"
#include "Player/TPSPlayerState.h"
#include "UI/TPSPlayerStatRowWidget.h"
#include "Components/VerticalBox.h"
#include "TPSUtils.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UTPSGameOverWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GetWorld())
	{
		const auto GameMode = Cast<ATPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &UTPSGameOverWidget::OnMatchStateChanged);
		}
	}

	if (ResetLevelButton)
	{
		ResetLevelButton->OnClicked.AddDynamic(this, &UTPSGameOverWidget::OnResetLevel);
	}
}

void UTPSGameOverWidget::OnMatchStateChanged(ETPSMatchState State)
{
	if (State == ETPSMatchState::GameOver)
	{
		UpdatePlayersStat();
	}
}

void UTPSGameOverWidget::UpdatePlayersStat()
{
	if (!GetWorld() || !PlayerStatBox) return;

	PlayerStatBox->ClearChildren();

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if (!Controller) continue;

		const auto PlayerState = Cast<ATPSPlayerState>(Controller->PlayerState);
		if (!PlayerState) continue;

		const auto PlayerStatRowWidget = CreateWidget<UTPSPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
		if (!PlayerStatRowWidget) continue;

		PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
		PlayerStatRowWidget->SetKills(TPSUtils::TextFromInt(PlayerState->GetKillsNum()));
		PlayerStatRowWidget->SetDeaths(TPSUtils::TextFromInt(PlayerState->GetDeathsNum()));
		PlayerStatRowWidget->SetTeam(TPSUtils::TextFromInt(PlayerState->GetTeamID()));
		PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());
		PlayerStatRowWidget->SetTeamColor(PlayerState->GetTeamColor());

		PlayerStatBox->AddChild(PlayerStatRowWidget);
	}
}

void UTPSGameOverWidget::OnResetLevel()
{
	const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	UGameplayStatics::OpenLevel(this, FName(CurrentLevelName)); // "Hard reset"
}
