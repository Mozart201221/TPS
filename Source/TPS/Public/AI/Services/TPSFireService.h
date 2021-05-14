// TPS. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TPSFireService.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UTPSFireService : public UBTService
{
	GENERATED_BODY()
	
public:
	UTPSFireService();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector EnemyActorKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
