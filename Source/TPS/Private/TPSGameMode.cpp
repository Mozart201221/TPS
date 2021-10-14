// TPS. All Rights Reserved


#include "TPSGameMode.h"
#include "Player/TPSCharacter.h"
#include "Player/TPSPlayerController.h"
#include "AIController.h"
#include "UI/TPSGameHUD.h"
#include "Player/TPSPlayerState.h"
#include "TPSUtils.h"
#include "Components/TPSRespawnComponent.h"
#include "Components/TPSWeaponComponent.h"
#include "EngineUtils.h"
#include "TPSGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogTPSGameMode, All, All);

constexpr static int32 MinRoundTimeForRespawn = 10;

ATPSGameMode::ATPSGameMode()
{
	DefaultPawnClass = ATPSCharacter::StaticClass();
	PlayerControllerClass = ATPSPlayerController::StaticClass();
	HUDClass = ATPSGameHUD::StaticClass();
	PlayerStateClass = ATPSPlayerState::StaticClass();
}

void ATPSGameMode::StartPlay()
{
	Super::StartPlay();

	SpawnBots();
	CreateTeamsInfo();

	CurrentRound = 1;
	StartRound();

	SetMatchState(ETPSMatchState::InProgress);
}

UClass* ATPSGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (InController && InController->IsA<AAIController>())
	{
		return AIPawnClass;
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}



void ATPSGameMode::SpawnBots()
{
	if (!GetWorld()) return;

	for (int32 i = 0; i < GameData.PlayersNum - 1; ++i)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		const auto TPSAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
		RestartPlayer(TPSAIController); 
	}
}

void ATPSGameMode::StartRound()
{
	RoundCountDown = GameData.RoundTime;
	GetWorldTimerManager().SetTimer(GameRoundTimeHadle, this, &ATPSGameMode::GameTimerUpdate, 1.0f, true);
}

void ATPSGameMode::GameTimerUpdate()
{
	//UE_LOG(LogTPSGameMode, Display, TEXT("Time: %i / Round: %i/%i"), RoundCountDown, CurrentRound, GameData.RoundsNum);

	const auto TimerRate = GetWorldTimerManager().GetTimerRate(GameRoundTimeHadle);

	if (--RoundCountDown == 0)
	{
		GetWorldTimerManager().ClearTimer(GameRoundTimeHadle);

		if (CurrentRound + 1 <= GameData.RoundsNum)
		{
				++CurrentRound;
				ResetPlayers();
				StartRound();
		}	
		else
		{
			GameOver();
		}
	}
}

void ATPSGameMode::ResetPlayers()
{
	if (!GetWorld()) return;

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		ResetOnePlayer(It->Get());
	}
}

void ATPSGameMode::ResetOnePlayer(AController* Controller)
{
	if (Controller && Controller->GetPawn())
	{
		Controller->GetPawn()->Reset();
	}

	RestartPlayer(Controller);
	SetPlayerColor(Controller);
}

void ATPSGameMode::CreateTeamsInfo()
{
	if (!GetWorld()) return;

	int32 TeamID = 1;
	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if (!Controller) continue;

		const auto PlayerState = Cast<ATPSPlayerState>(Controller->PlayerState);
		if (!PlayerState) continue;

		PlayerState->SetTeamID(TeamID);
		PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
		PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
		SetPlayerColor(Controller);

		TeamID = TeamID == 1 ? 2 : 1;
	}
}

FLinearColor ATPSGameMode::DetermineColorByTeamID(int32 TeamID) const
{
	if (TeamID - 1 < GameData.TeamColors.Num())
	{
		return GameData.TeamColors[TeamID - 1];
	}
	UE_LOG(LogTPSGameMode, Warning, TEXT("No color for team id: %i, set to default: %s"), TeamID, *GameData.DefaultTeamColor.ToString());
	return GameData.DefaultTeamColor;
}

void ATPSGameMode::SetPlayerColor(AController* Controller)
{
	if (!Controller) return;

	const auto Character = Cast<ATPSCharacter>(Controller->GetPawn());
	if (!Character) return;

	const auto PlayerState = Cast<ATPSPlayerState>(Controller->PlayerState);
	if (!PlayerState) return;

	Character->SetPlayerColor(PlayerState->GetTeamColor());
}



void ATPSGameMode::Killed(AController* KillerController, AController* VictimController)
{
	const auto KillerPlayerState = KillerController ? Cast<ATPSPlayerState>(KillerController->PlayerState) : NULL;
	const auto VictimPlayerState = VictimController ? Cast<ATPSPlayerState>(VictimController->PlayerState) : NULL;

	if (KillerPlayerState)
	{
		KillerPlayerState->AddKill();
	}

	if (VictimPlayerState)
	{
		VictimPlayerState->AddDeath();
	}

	StartRespawn(VictimController);
}

void ATPSGameMode::LogPlayerInfo()
{
	if (!GetWorld()) return;

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if (!Controller) continue;

		const auto PlayerState = Cast<ATPSPlayerState>(Controller->PlayerState);
		if (!PlayerState) continue;

		PlayerState->LogInfo();
	}
}

void ATPSGameMode::StartRespawn(AController* Controller)
{
	const auto RespawnAvailable = RoundCountDown > MinRoundTimeForRespawn + GameData.RespawnTime;
	if (!RespawnAvailable) return;
	const auto RespawnComponent = TPSUtils::GetTPSPlayerComponent<UTPSRespawnComponent>(Controller);
	if (!RespawnComponent) return;

	RespawnComponent->Respawn(GameData.RespawnTime);
}

void ATPSGameMode::RespawnRequest(AController* Controller)
{
	ResetOnePlayer(Controller);
}

void ATPSGameMode::GameOver()
{
	UE_LOG(LogTPSGameMode, Display, TEXT("============= GAME OVER ==========="));
	LogPlayerInfo();

	for (auto Pawn : TActorRange<APawn>(GetWorld()))
	{
		if (Pawn)
		{
			Pawn->TurnOff();
			Pawn->DisableInput(NULL);
		}
	}

	SetMatchState(ETPSMatchState::GameOver);
}

void ATPSGameMode::SetMatchState(ETPSMatchState State)
{
	if (MatchState == State) return;

	MatchState = State;
	OnMatchStateChanged.Broadcast(MatchState);
}

bool ATPSGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);

	if(PauseSet)
	{
		StopAllFire();
		SetMatchState(ETPSMatchState::Pause);
	}

	return PauseSet;
}

bool ATPSGameMode::ClearPause()
{
	const auto PauseCleared = Super::ClearPause();
	if (PauseCleared)
	{
		SetMatchState(ETPSMatchState::InProgress);
	}

	return PauseCleared;
}

void ATPSGameMode::StopAllFire()
{
	for (auto Pawn : TActorRange<APawn>(GetWorld()))
	{
		const auto WeaponComponent = TPSUtils::GetTPSPlayerComponent<UTPSWeaponComponent>(Pawn);
		if (!WeaponComponent) continue;

		WeaponComponent->StopFire();
		WeaponComponent->Zoom(false);
	}
}