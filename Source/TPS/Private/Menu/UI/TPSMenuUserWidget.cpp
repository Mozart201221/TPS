// TPS. All Rights Reserved


#include "Menu/UI/TPSMenuUserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "TPSGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HorizontalBox.h"
#include "Menu/UI/TPSLevelItemWidget.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogTPSMenuWidget, All, All);

void UTPSMenuUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &UTPSMenuUserWidget::OnStartGame);
	}

	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &UTPSMenuUserWidget::OnQuitGame);
	}

	InitLevelItems();
}

void UTPSMenuUserWidget::InitLevelItems()
{
	const auto TPSGameInstance = GetTPSGameInstance();
	if (!TPSGameInstance) return;

	checkf(TPSGameInstance->GetLevelsData().Num() != 0, TEXT("Levels data must not be empty!"));

	if (!LevelItemsBox) return;
	LevelItemsBox->ClearChildren();

	for (auto LevelData : TPSGameInstance->GetLevelsData())
	{
		const auto LevelItemWidget = CreateWidget<UTPSLevelItemWidget>(GetWorld(), LevelItemWidgetClass);
		if (!LevelItemWidget) continue;

		LevelItemWidget->SetLevelData(LevelData);
		LevelItemWidget->OnLevelSelected.AddUObject(this, &UTPSMenuUserWidget::OnLevelSelected);

		LevelItemsBox->AddChild(LevelItemWidget);
		LevelItemWidgets.Add(LevelItemWidget);
	}
	if (TPSGameInstance->GetStartupLevel().LevelName.IsNone())
	{
		OnLevelSelected(TPSGameInstance->GetLevelsData()[0]);
	}
	else
	{
		OnLevelSelected(TPSGameInstance->GetStartupLevel());
	}

}

void UTPSMenuUserWidget::OnLevelSelected(const FLevelData& Data)
{
	const auto TPSGameInstance = GetTPSGameInstance();
	if (!TPSGameInstance) return;

	TPSGameInstance->SetStartupLevel(Data);

	for (auto LevelItemWidget : LevelItemWidgets)
	{
		if (LevelItemWidget)
		{
			const auto IsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;
			LevelItemWidget->SetSelected(IsSelected);
		}
	}
}


void UTPSMenuUserWidget::OnStartGame()
{
	PlayAnimation(HideAnimation);
	UGameplayStatics::PlaySound2D(GetWorld(), StartGameSound);
}

void UTPSMenuUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (Animation != HideAnimation) return;

	const auto TPSGameInstance = GetTPSGameInstance();
	if (!TPSGameInstance) return;

	UGameplayStatics::OpenLevel(this, TPSGameInstance->GetStartupLevel().LevelName);
}

void UTPSMenuUserWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

UTPSGameInstance* UTPSMenuUserWidget::GetTPSGameInstance() const
{
	if (!GetWorld()) return NULL;
	return GetWorld()->GetGameInstance<UTPSGameInstance>();
}