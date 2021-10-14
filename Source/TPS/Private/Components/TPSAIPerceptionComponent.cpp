// TPS. All Rights Reserved


#include "Components/TPSAIPerceptionComponent.h"
#include "AIController.h"
#include "TPSUtils.h"
#include "Components/TPSHealthComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"

AActor* UTPSAIPerceptionComponent::GetClosestEnemy() const
{
	TArray<AActor*> PercieveActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);
	if (PercieveActors.Num() == 0)
	{
		GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), PercieveActors);
		if (PercieveActors.Num() == 0)
		return NULL;
	}

	const auto Controller = Cast<AAIController>(GetOwner());
	if (!Controller) return NULL;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return NULL;

	float BestDistance = MAX_FLT;
	AActor* BestPawn = NULL;
	for (const auto PercieveActor : PercieveActors)
	{
		const auto HealthComponent = TPSUtils::GetTPSPlayerComponent<UTPSHealthComponent>(PercieveActor);

		const auto PercievePawn = Cast<APawn>(PercieveActor);
		const auto AreEnemies = PercievePawn && TPSUtils::AreEnemies(Controller, PercievePawn->Controller);

		if (HealthComponent && !HealthComponent->IsDead() && AreEnemies)
		{
			const auto CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
			if (CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn = PercieveActor;
			}
		}
	}
	return BestPawn;
};

