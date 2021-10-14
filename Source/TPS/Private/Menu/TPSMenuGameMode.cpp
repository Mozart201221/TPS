// TPS. All Rights Reserved


#include "Menu/TPSMenuGameMode.h"
#include "Menu/TPSMenuPlayerController.h"
#include "Menu/UI/TPSMenuHUD.h"

ATPSMenuGameMode::ATPSMenuGameMode()
{
	PlayerControllerClass = ATPSMenuPlayerController::StaticClass();
	HUDClass = ATPSMenuHUD::StaticClass();
}

