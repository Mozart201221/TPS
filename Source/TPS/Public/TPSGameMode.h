// TPS. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPSCoreTypes.h"
#include "TPSGameMode.generated.h"

class AAIController;

UCLASS()
class TPS_API ATPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATPSGameMode();

	FOnMatchStateChangedSignature OnMatchStateChanged;

	virtual void StartPlay() override;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	void Killed(AController* KillerController, AController* VictimController);

	FGameData GetGameData() const { return GameData; }
	int32 GetCurrentRoundNum() const { return CurrentRound; }
	int32 GetRoundSecondsRemaining() const { return RoundCountDown; }

	void RespawnRequest(AController* Controller);

	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;

	virtual bool ClearPause() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<APawn> AIPawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FGameData GameData;

private:
	ETPSMatchState MatchState = ETPSMatchState::WaitingToStart;

	int32 CurrentRound = 1;
	int32 RoundCountDown = 0;
	FTimerHandle GameRoundTimeHadle;

	void SpawnBots();
	void StartRound();
	void GameTimerUpdate();

	void ResetPlayers();
	void ResetOnePlayer(AController* Controller);

	void CreateTeamsInfo();
	FLinearColor DetermineColorByTeamID(int32 TeamID) const;
	void SetPlayerColor(AController* Controller);

	void LogPlayerInfo();

	void StartRespawn(AController* Controller);

	void GameOver();

	void SetMatchState(ETPSMatchState State);

	void StopAllFire();
};
