// TPS. All Rights Reserved


#include "Player/TPSPlayerController.h"
#include "Components/TPSRespawnComponent.h"
#include "TPSGameMode.h"
#include "TPSGameInstance.h"

ATPSPlayerController::ATPSPlayerController()
{
	RespawnComponent = CreateDefaultSubobject<UTPSRespawnComponent>("RespawnComponent");
}

void ATPSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OnNewPawn.Broadcast(InPawn);
}

void ATPSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld())
	{
		if (const auto GameMode = Cast<ATPSGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &ATPSPlayerController::OnMatchStateChanged);
		}
	}
}

void ATPSPlayerController::OnMatchStateChanged(ETPSMatchState State)
{
	if (State == ETPSMatchState::InProgress)
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
}



void ATPSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (!InputComponent) return;

	InputComponent->BindAction("PauseGame", IE_Pressed, this, &ATPSPlayerController::OnPauseGame);
	InputComponent->BindAction("Mute", IE_Pressed, this, &ATPSPlayerController::OnMuteSound);
}

void ATPSPlayerController::OnPauseGame()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

	GetWorld()->GetAuthGameMode()->SetPause(this);
}

void ATPSPlayerController::OnMuteSound()
{
	if (!GetWorld()) return;

	const auto TPSGameInstance = GetWorld()->GetGameInstance<UTPSGameInstance>();
	if (!TPSGameInstance) return;

	TPSGameInstance->ToggleVolume();
}