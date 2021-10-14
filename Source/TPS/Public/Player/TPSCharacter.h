// TPS. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSCharacter.generated.h"

class UTPSHealthComponent;
class UTPSWeaponComponent;
class USoundCue;


UCLASS()
class TPS_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSCharacter(const FObjectInitializer& ObjInit);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTPSHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTPSWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	FName MaterialColorName = "BodyColor";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* DeathSound;

	virtual void BeginPlay() override;
	virtual void OnDeath();

	virtual void OnHealthChanged(float Health, float HealthDelta);

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void TurnOff() override;
	virtual void Reset() override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual bool IsRunning() const;

	void SetPlayerColor(const FLinearColor& Color);
};
