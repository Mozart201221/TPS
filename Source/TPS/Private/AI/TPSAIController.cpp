// TPS. All Rights Reserved


#include "AI/TPSAIController.h"
#include "AI/TPSAICharacter.h"
#include "Components/TPSAIPerceptionComponent.h"
#include "Components/TPSRespawnComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
 
ATPSAIController::ATPSAIController()
{
	TPSAIPerceptionComponent = CreateDefaultSubobject<UTPSAIPerceptionComponent>("TPSAIPerceptionComponent");
	SetPerceptionComponent(*TPSAIPerceptionComponent);

	RespawnComponent = CreateDefaultSubobject<UTPSRespawnComponent>("RespawnComponent");

	bWantsPlayerState = true;
}

void ATPSAIController::OnPossess(APawn* InPawn) 
{
	Super::OnPossess(InPawn);

	const auto TPSCharacter = Cast<ATPSAICharacter>(InPawn);
	if (TPSCharacter)
	{
		RunBehaviorTree(TPSCharacter->BehaviorTreeAsset);
	}
}

void ATPSAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const auto AimActor = GetFocusOnActor();
	SetFocus(AimActor);
}

AActor* ATPSAIController::GetFocusOnActor() const
{
	if (!GetBlackboardComponent()) return NULL;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
