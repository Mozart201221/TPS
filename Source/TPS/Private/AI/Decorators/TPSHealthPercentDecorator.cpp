// TPS. All Rights Reserved


#include "AI/Decorators/TPSHealthPercentDecorator.h"
#include "AIController.h"
#include "TPSUtils.h"
#include "Components/TPSHealthComponent.h"

UTPSHealthPercentDecorator::UTPSHealthPercentDecorator()
{
	NodeName = "Health Percent";
}

bool UTPSHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = OwnerComp.GetAIOwner();
	if(!Controller)	return false;

	const auto HealthComponent = TPSUtils::GetTPSPlayerComponent<UTPSHealthComponent>(Controller->GetPawn());
	if (!HealthComponent || HealthComponent->IsDead())	return false;

	return HealthComponent->GetHealthPercent() <= HealthPercent;
}
