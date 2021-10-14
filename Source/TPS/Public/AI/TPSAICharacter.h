// TPS. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/TPSCharacter.h"
#include "TPSAICharacter.generated.h"

class UBehaviorTree;
class UWidgetComponent;

UCLASS()
class TPS_API ATPSAICharacter : public ATPSCharacter
{
	GENERATED_BODY()
	
public:
	ATPSAICharacter(const FObjectInitializer& ObjInit);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	float HealthVisibilityDistance = 1000.0f;

	virtual void BeginPlay() override;

	virtual void OnDeath() override;
	virtual void OnHealthChanged(float Health, float HealthDelta) override;

private:
	void UpdateHealthWidgetVisibility();
};
