// TPS. All Rights Reserved


#include "UI/TPSBaseWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void UTPSBaseWidget::Show()
{
	PlayAnimation(ShowAnimation);
	UGameplayStatics::PlaySound2D(GetWorld(), OpenSound);
}