// TPS. All Rights Reserved


#include "Menu/TPSMenuPlayerController.h"
#include "TPSGameInstance.h"

void ATPSMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;

}