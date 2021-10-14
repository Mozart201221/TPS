// TPS. All Rights Reserved


#include "Menu/UI/TPSMenuHUD.h"
#include "UI/TPSBaseWidget.h"

void ATPSMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MenuWidgetClass)
	{
		const auto MenuWidget = CreateWidget<UTPSBaseWidget>(GetWorld(), MenuWidgetClass);
		if (MenuWidget)
		{
			MenuWidget->AddToViewport();
			MenuWidget->Show();
		}
	}
}