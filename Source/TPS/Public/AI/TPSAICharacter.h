// TPS. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/TPSCharacter.h"
#include "TPSAICharacter.generated.h"

class UBehaviorTree;

UCLASS()
class TPS_API ATPSAICharacter : public ATPSCharacter
{
	GENERATED_BODY()
	
public:
	ATPSAICharacter(const FObjectInitializer& ObjInit);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

protected:
	virtual void OnDeath() override;
};
