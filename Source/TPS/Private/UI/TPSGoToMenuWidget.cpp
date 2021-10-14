// TPS. All Rights Reserved


#include "UI/TPSGoToMenuWidget.h"
#include "Components/Button.h"
#include "TPSGameInstance.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogTPSGoToMenuWidget, All, All);

void UTPSGoToMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GoToMenuButton)
	{
		GoToMenuButton->OnClicked.AddDynamic(this, &UTPSGoToMenuWidget::OnGoToMenu);
	}
}

void UTPSGoToMenuWidget::OnGoToMenu()
{
	if (!GetWorld()) return;

	const auto TPSGameInstance = GetWorld()->GetGameInstance<UTPSGameInstance>();
	if (!TPSGameInstance) return;

	if (TPSGameInstance->GetMenuLevelName().IsNone())
	{
		UE_LOG(LogTPSGoToMenuWidget, Error, TEXT("Menu level name is NONE"));
		return;
	}

	UGameplayStatics::OpenLevel(this, TPSGameInstance->GetMenuLevelName());
}