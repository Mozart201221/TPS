// TPS. All Rights Reserved


#include "Menu/UI/TPSLevelItemWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UTPSLevelItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (LevelSelectButton)
	{
		LevelSelectButton->OnClicked.AddDynamic(this, &UTPSLevelItemWidget::OnLevelItemClicked);
		LevelSelectButton->OnHovered.AddDynamic(this, &UTPSLevelItemWidget::OnLevelItemHovered);
		LevelSelectButton->OnUnhovered.AddDynamic(this, &UTPSLevelItemWidget::OnLevelItemUnhovered);
	}
}

void UTPSLevelItemWidget::OnLevelItemClicked()
{
	OnLevelSelected.Broadcast(LevelData);
}

void UTPSLevelItemWidget::SetLevelData(const FLevelData& Data)
{
	LevelData = Data;

	if (LevelNameTextBlock)
	{
		LevelNameTextBlock->SetText(FText::FromName(Data.LevelDisplayName));
	}

	if (LevelImage)
	{
		LevelImage->SetBrushFromTexture(Data.LevelThumb);
	}
}

void UTPSLevelItemWidget::SetSelected(bool IsSelected)
{
	if (LevelImage)
	{
		LevelImage->SetColorAndOpacity(IsSelected ? FLinearColor::Red : FLinearColor::White);
	}
}

void UTPSLevelItemWidget::OnLevelItemHovered()
{
	if (FrameImage)
	{
		FrameImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UTPSLevelItemWidget::OnLevelItemUnhovered()
{
	if (FrameImage)
	{
		FrameImage->SetVisibility(ESlateVisibility::Hidden);
	}
}
