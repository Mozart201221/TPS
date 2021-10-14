// TPS. All Rights Reserved


#include "UI/TPSGameHUD.h"
#include "Engine/Canvas.h"
#include "UI/TPSBaseWidget.h"
#include "TPSGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogTPSGameHUD, All, All);

void ATPSGameHUD::DrawHUD()
{
	Super::DrawHUD();

	//DrawCrossHair();
}

void ATPSGameHUD::BeginPlay()
{
	Super::BeginPlay();
	
	GameWidgets.Add(ETPSMatchState::InProgress, CreateWidget<UTPSBaseWidget>(GetWorld(), PlayerHUDWidgetClass));
	GameWidgets.Add(ETPSMatchState::Pause, CreateWidget<UTPSBaseWidget>(GetWorld(), PauseWidgetClass));
	GameWidgets.Add(ETPSMatchState::GameOver, CreateWidget<UTPSBaseWidget>(GetWorld(), GameOverWidgetClass));

	for (auto GameWidgetPair : GameWidgets)
	{
		const auto GameWidget = GameWidgetPair.Value;
		if (!GameWidget) continue;

		GameWidget->AddToViewport();
		GameWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (GetWorld())
	{
		const auto GameMode = Cast<ATPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &ATPSGameHUD::OnMatchStateChanged);
		}
	}
}

void ATPSGameHUD::OnMatchStateChanged(ETPSMatchState State)
{
	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (GameWidgets.Contains(State))
	{
		CurrentWidget = GameWidgets[State];
	}

	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Visible);
		CurrentWidget->Show();
	}
	
	UE_LOG(LogTPSGameHUD, Display, TEXT("Match state changed: %s"), *UEnum::GetValueAsString(State));
}

void ATPSGameHUD::DrawCrossHair()
{
	const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

	const float HalfLineSize = 10.0f;
	const float LineThickness = 2.0f;
	const FLinearColor LineColor = FLinearColor::Green;

	DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
	DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}
