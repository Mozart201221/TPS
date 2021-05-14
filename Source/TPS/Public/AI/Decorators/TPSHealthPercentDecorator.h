// TPS. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "TPSHealthPercentDecorator.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UTPSHealthPercentDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UTPSHealthPercentDecorator();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float HealthPercent = 0.6f;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
